#include "World.h"
#include <Lengine/Camera3d.h>


World::World() {
	_needsChunkUpdate = true;
	
}

World::~World() {}

void World::init(int worldWidth, int worldDepth, int &renderDist) {
	_worldWidth = worldWidth;
	_worldDepth = worldDepth;
	_renderDistance = renderDist;	
}

void World::updateWorld(glm::vec3& cameraPosition, glm::vec3 cameraDirection, bool &LeftMouseDown, bool& RightMouseDown, bool &applyGravity, int8_t blockType) {
	_cameraPosition = cameraPosition;
	applyCollison(cameraPosition, applyGravity);
	blockPlaceOrDestroy(cameraPosition, cameraDirection, LeftMouseDown, RightMouseDown, blockType);

}
void World::renderWorld(Lengine::GLSLProgram& textureProgram) {

	_startX = (_cameraPosition.x / CHUNK_SIZE - _renderDistance > 0)? int(_cameraPosition.x / CHUNK_SIZE - _renderDistance): 0;
	_startZ = (_cameraPosition.z / CHUNK_SIZE - _renderDistance > 0) ? int(_cameraPosition.z / CHUNK_SIZE - _renderDistance) : 0;

	_endX = (_cameraPosition.x / CHUNK_SIZE + _renderDistance > _worldWidth) ? int(_worldWidth) : _cameraPosition.x / CHUNK_SIZE + _renderDistance;
	_endZ = (_cameraPosition.z / CHUNK_SIZE + _renderDistance > _worldWidth) ? int(_worldWidth) : _cameraPosition.z / CHUNK_SIZE + _renderDistance;


	for (auto it = _chunkLoadList.begin(); it != _chunkLoadList.end(); ) {
		int chunkKey = *it;
		int x = chunkKey / LARGE_PRIME;
		int z = chunkKey - x * LARGE_PRIME;

		if (x < _startX || x >= _endX || z < _startZ || z >= _endZ) {
			it = _chunkLoadList.erase(it);
			removeChunk(x, z);
			
		}
		else {
			++it;
		}
	}


	for (int x = _startX; x < _endX; x++) {
		for (int z = _startZ; z < _endZ; z++) {
			int targetChunkKey = x*LARGE_PRIME + z;
			
			if (_chunkLoadList.find(targetChunkKey) == _chunkLoadList.end()) {
				_chunkLoadList.insert(targetChunkKey);				 
				
				_chunks[x][z] = std::make_unique<Chunk>();

					loadOrGenerateChunk("world", x * CHUNK_SIZE, z * CHUNK_SIZE, _chunks[x][z]);
					_chunks[x][z]->generateCubes();
			}
			
			glPushMatrix();
			glTranslatef(x * CHUNK_SIZE, 0, z * CHUNK_SIZE);
			_chunks[x][z]->render(textureProgram);
			glPopMatrix();
		}
	}
}

void World::removeChunk(int x, int z) {
	auto itX = _chunks.find(x);
	if (itX != _chunks.end()) {
		auto itZ = itX->second.find(z);
		if (itZ != itX->second.end()) {
			itZ->second->destroy(); 
			itX->second.erase(itZ); 
		}
		if (itX->second.empty()) {
			_chunks.erase(itX); // Remove empty column
		}
	}
}

void World::loadOrGenerateChunk(const std::string& worldSavePath, int x, int z, std::unique_ptr<Chunk>& chunk)
{
	
	std::string filepath = worldSavePath + "/" + std::to_string(x) + "_" + std::to_string(z) + ".bin";
	FILE* fp = nullptr;
	fopen_s(&fp, filepath.c_str(), "rb");

	if (fp) {
		
		fclose(fp);
		chunk->loadChunk(worldSavePath, x, z);
	
	}
	else {
	
		chunk->init(x / CHUNK_SIZE, 0, z / CHUNK_SIZE);
	
	}
}

void World::applyCollison(glm::vec3& cameraPosition, bool& applyGravity) {
	float minDist = 1.0f;
	glm::vec3 collisionDepth = (cameraPosition - glm::vec3(1, 1, 1)) - glm::vec3((int)_cameraPosition.x, (int)_cameraPosition.y, (int)_cameraPosition.z);
	glm::vec3 collisionDepth2 = (cameraPosition - glm::vec3(0, 0, 0)) - glm::vec3((int)_cameraPosition.x, (int)_cameraPosition.y, (int)_cameraPosition.z);

	if (_chunks[_cameraPosition.x / CHUNK_SIZE][_cameraPosition.z / CHUNK_SIZE]->getBlock((_cameraPosition.x) % 16, (_cameraPosition.y - 1) % 127, (_cameraPosition.z) % 16) != 0) {
		cameraPosition.y -= collisionDepth.y;
	}

	if (_chunks[_cameraPosition.x / CHUNK_SIZE][_cameraPosition.z / CHUNK_SIZE]->getBlock((_cameraPosition.x) % 16, (_cameraPosition.y + 1) % 127, (_cameraPosition.z) % 16) != 0) {
		std::cout << collisionDepth2.y << std::endl;
		cameraPosition.y -= collisionDepth2.y;

	}
}

void World::blockPlaceOrDestroy(glm::vec3& cameraPosition, glm::vec3 cameraDirection, bool& LeftMouseDown, bool& RightMouseDown, int8_t blockType) {
	if (LeftMouseDown || RightMouseDown) {
		GameElements::RayHit ray = _gameElements.castRay(cameraPosition, cameraDirection, 1, _chunks);

		if (ray.hit)
		{
			glm::ivec3 adjBlockPos = ray.adjacentPos;
			glm::ivec3 BlockPos = ray.blockPos;
			if (RightMouseDown) {
				_chunks[adjBlockPos.x / CHUNK_SIZE][adjBlockPos.z / CHUNK_SIZE]->setBlock(adjBlockPos.x % 16, adjBlockPos.y % 127, adjBlockPos.z % 16, blockType);
				_chunks[adjBlockPos.x / CHUNK_SIZE][adjBlockPos.z / CHUNK_SIZE]->saveChunk("world");
				std::cout << adjBlockPos.x <<" "<<adjBlockPos.z<< std::endl;

			}
			
			if (LeftMouseDown) {
				_chunks[BlockPos.x / CHUNK_SIZE][BlockPos.z / CHUNK_SIZE]->destroyBlock(BlockPos.x % 16, BlockPos.y % 127, BlockPos.z % 16, 0);
				_chunks[BlockPos.x / CHUNK_SIZE][BlockPos.z / CHUNK_SIZE]->saveChunk("world");
				std::cout << "save" << std::endl;

			}


			_chunks[adjBlockPos.x / CHUNK_SIZE][adjBlockPos.z / CHUNK_SIZE]->updateChunk();
			_chunks[BlockPos.x / CHUNK_SIZE][BlockPos.z / CHUNK_SIZE]->updateChunk();

			LeftMouseDown = false;
			RightMouseDown = false;
		}


	}


}










