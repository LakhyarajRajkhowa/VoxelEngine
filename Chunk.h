#pragma once

#include <cstdint>

#include <vector>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp> 

#include <Lengine/GLSLProgram.h>
#include <Lengine/GLTexture.h>
#include <Lengine/ResourceManager.h>
#include <Lengine/SimplexNoise.h>


//#include "ChunkSerialisation.h"



 const uint8_t CHUNK_SIZE = 16; // 16x16x16 chunk
 const int8_t CHUNK_SIZE_Y = 127; 
 const SimplexNoise generator;
 const float incrementSize = 500;
// const int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

 


class Chunk {
public:
    Chunk();
    void init(int chunkX, int chunkY, int chunkZ);
    void generateCubes();
    void setChunkPosition(int x, int y, int z);
    void generateChunk();  // Generates voxel data
    void generateMesh();   // Converts voxel data to OpenGL mesh
    void render(Lengine::GLSLProgram& shader);

    bool isChunkLoaded;
    
    //void loadChunk() { isChunkLoaded = true;  }
    void destroy();

    void saveChunk(const std::string& filename);
    void loadChunk(const std::string& filename, int x, int z);

    int8_t getBlock(int8_t blockPositionX, int8_t blockPositionY, int8_t blockPositionZ) { return _blocks[blockPositionX][blockPositionY][blockPositionZ]; }
    void setBlock(int8_t blockPositionX, int8_t blockPositionY, int8_t blockPositionZ, int8_t blockType) { _blocks[blockPositionX][blockPositionY][blockPositionZ] = blockType;  }
    void destroyBlock(int8_t blockPositionX, int8_t blockPositionY, int8_t blockPositionZ, int8_t blockType) { _blocks[blockPositionX][blockPositionY][blockPositionZ] = 0; }

    void updateChunk() { _needsChunkUpdate = true;   generateMesh(); 
     setAttribute(); }

private:

    glm::ivec3 _chunkPosition;
    std::vector<GLuint> VAO;
    std::vector<GLuint> VBO;

   
    bool _needsChunkUpdate;
    std::vector<std::vector<uint8_t>> _vertices;
 
    
    //Block _blocks[CHUNK_SIZE][CHUNK_SIZE_Y][CHUNK_SIZE];
    int8_t _blocks[CHUNK_SIZE][CHUNK_SIZE_Y][CHUNK_SIZE];

    //std::vector<std::vector<std::vector<int8_t>>> _blocks;
    bool isBlockVisible(uint8_t x, uint8_t y, uint8_t z);
    void addFace(std::vector<uint8_t>& verts, glm::u8vec3 pos, glm::u8vec3 normal, int8_t blockType);

    void setAttribute();

   
    
};



