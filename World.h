#pragma once

#include <vector>
#include <unordered_set>
#include <memory>
#include "Chunk.h"
#include <Lengine/InputManager.h>
#include "GameElements.h"

#define LARGE_PRIME 9973


class World
{
public:
	World();
	~World();
	void init(int worldWidth, int worldDepth, int& renderDist);
	void renderWorld(Lengine::GLSLProgram& textureProgram);
	void updateWorld(glm::vec3& cameraPosition, glm::vec3 cameraDirection, bool& LeftMouseDown, bool& RightMouseDown, bool& applyGravity, int8_t blockType);
private:
	int _worldWidth;
	int _worldDepth;
	int _renderDistance;
	bool _needsChunkUpdate;
	float gravitySpeedY = 0;

	int _startX, _endX, _startZ, _endZ;

	glm::ivec3 _cameraPosition;

	//std::vector<std::vector<std::unique_ptr<Chunk>>> _chunks;
	std::unordered_map<int, std::unordered_map<int, std::unique_ptr<Chunk>>> _chunks;
 
	std::unordered_set<int> _chunkLoadList;
	std::unordered_set<int> _chunkSaveList;


	void loadOrGenerateChunk(const std::string& worldSavePath, int x, int z, std::unique_ptr<Chunk>& chunk);
	void removeChunk(int x, int z);
	void applyCollison(glm::vec3& cameraPosition, bool& applyGravity);
	void blockPlaceOrDestroy(glm::vec3& cameraPosition, glm::vec3 cameraDirection, bool& LeftMouseDown, bool& RightMouseDown, int8_t blockType);

	GameElements _gameElements;


};

