

#include "Chunk.h"
#include <iostream>
#include <fstream>

#include <sys/stat.h>
#include <direct.h>
#include <zlib/zlib-1.3.1/zlib.h>

Chunk::Chunk() {

    isChunkLoaded = true;
    _needsChunkUpdate = true;
}


void Chunk::init(int chunkX, int chunkY, int chunkZ) {
    if (isChunkLoaded) {
        setChunkPosition(chunkX, chunkY, chunkZ);
        generateChunk();
        saveChunk("world");
        isChunkLoaded = false;
    }
    
}
void Chunk::generateCubes() {
    
        generateMesh();
        setAttribute();
       
    

}
float mapRange(float value, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
}
void Chunk::setChunkPosition(int x, int y, int z) {
    _chunkPosition = glm::ivec3(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE);
}

void Chunk::generateChunk() {
   
    for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
        for (uint8_t z = 0; z < CHUNK_SIZE; z++) {

            float maxHeightDirt = mapRange(generator.fractal(7, (x + _chunkPosition.x) / incrementSize, (z + _chunkPosition.z) / incrementSize), -1, 1, 0, 1) * 64;

            float maxHeightStone = mapRange(generator.fractal(7, (x + _chunkPosition.x) / incrementSize, (z + _chunkPosition.z) / incrementSize), -1, 1, 0, 1) * 64;


            for (uint8_t y = 10; y < CHUNK_SIZE_Y; y++) {
                int8_t intMaxHeightStone = (maxHeightStone * maxHeightDirt) / 32;
                if (y < intMaxHeightStone) {
                    _blocks[x][y][z] = 10;
                }
                // if (sqrt((float)(x - CHUNK_SIZE / 2) * (x - CHUNK_SIZE / 2) + (y - CHUNK_SIZE / 2) * (y - CHUNK_SIZE / 2) + (z - CHUNK_SIZE / 2) * (z - CHUNK_SIZE / 2)) <= CHUNK_SIZE / 2)
                else if (y < (int8_t)maxHeightDirt && y >= intMaxHeightStone) {
                    _blocks[x][y][z] = 4;
                }
                else  if (y == (int8_t)maxHeightDirt) {
                    _blocks[x][y][z] = 1;
                }
                else
                    _blocks[x][y][z] = 0;
            }

        }
    }



}

bool Chunk::isBlockVisible(uint8_t x, uint8_t y, uint8_t z) {
    if (x <= 0 || y <= 0 || z <= 0 || x >= CHUNK_SIZE - 1 || y >= CHUNK_SIZE_Y - 1 || z >= CHUNK_SIZE - 1)
        return true;
    return _blocks[x][y][z] && (
        !_blocks[x + 1][y][z] || !_blocks[x - 1][y][z] ||
        !_blocks[x][y + 1][z] || !_blocks[x][y - 1][z] ||
        !_blocks[x][y][z + 1] || !_blocks[x][y][z - 1]
        );
}

void Chunk::generateMesh() {
    if (_needsChunkUpdate) {
        _vertices.resize(6);
        for (auto& vert : _vertices) vert.clear();

        for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
            for (uint8_t y = 0; y < CHUNK_SIZE_Y; y++) {
                for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
                    if (_blocks[x][y][z] && isBlockVisible(x, y, z)) {

                        if (_blocks[x][y][z] && !_blocks[x][y + 1][z]) {
                            addFace(_vertices[0], glm::u8vec3(x, y + 1, z), glm::u8vec3(0, 1, 0), _blocks[x][y][z]); // Top
                        }

                        if (_blocks[x][y][z] && !_blocks[x][y - 1][z]) {
                            addFace(_vertices[1], glm::u8vec3(x, y, z), glm::u8vec3(0, -1, 0), _blocks[x][y][z] + 2); // Bottom
                        }

                        if (_blocks[x][y][z] && !_blocks[x][y][z + 1]) {
                            addFace(_vertices[5], glm::u8vec3(x, y, z + 1), glm::u8vec3(0, 0, 1), _blocks[x][y][z] + 1); // Back
                        }
                        if(z == 15)  addFace(_vertices[5], glm::u8vec3(x, y, z + 1), glm::u8vec3(0, 0, 1), _blocks[x][y][z] + 1); // Back

                        if (_blocks[x][y][z] && !_blocks[x][y][z - 1]) {
                        addFace(_vertices[4], glm::u8vec3(x, y, z), glm::u8vec3(0, 0, -1), _blocks[x][y][z] + 1); // Front
                        }
                        if (z == 0)  addFace(_vertices[4], glm::u8vec3(x, y, z), glm::u8vec3(0, 0, -1), _blocks[x][y][z] + 1); // Front
                        

                        if (x  >= 0 && x  + 1 < CHUNK_SIZE && _blocks[x][y][z] && !_blocks[x + 1][y][z]) {                           
                            addFace(_vertices[3], glm::u8vec3(x + 1, y, z), glm::u8vec3(1, 0, 0), _blocks[x][y][z] + 1); // Right
                        }
                        if(x == 15) addFace(_vertices[3], glm::u8vec3(x + 1, y, z), glm::u8vec3(1, 0, 0), _blocks[x][y][z] + 1); // Right

                        if (x - 1 >= 0 && x  < CHUNK_SIZE && _blocks[x][y][z] && !_blocks[x - 1][y][z]) {
                        addFace(_vertices[2], glm::u8vec3(x, y, z), glm::u8vec3(-1, 0, 0), _blocks[x][y][z] + 1); // Left
                         }

                        if(x == 0) addFace(_vertices[2], glm::u8vec3(x, y, z), glm::u8vec3(-1, 0, 0), _blocks[x][y][z] + 1); // Left

                    }
                }
            }
        }
        _needsChunkUpdate = false;
    }
    
}

void Chunk::addFace(std::vector<uint8_t>& verts, glm::u8vec3 pos, glm::u8vec3 normal, int8_t blockType) {
    uint8_t size = 1;
    glm::u8vec3 tangent, bitangent;
    if (normal.x != 0) {
        tangent = glm::u8vec3(0, 0, 1);
        bitangent = glm::u8vec3(0, 1, 0);
    }
    else if (normal.y != 0) {
        tangent = glm::u8vec3(1, 0, 0);
        bitangent = glm::u8vec3(0, 0, 1);
    }
    else {
        tangent = glm::u8vec3(1, 0, 0);
        bitangent = glm::u8vec3(0, 1, 0);
    }

    glm::u8vec3 v0 = pos;
    glm::u8vec3 v1 = pos + tangent * size;
    glm::u8vec3 v2 = pos + bitangent * size;
    glm::u8vec3 v3 = pos + tangent * size + bitangent * size;

    uint8_t uv1 = (blockType % 51 - 1) * 5; // 5
    uint8_t uv2 = 5 * (51 - (blockType / 51 + 1)); // 250
    uint8_t uv3 = (blockType % 51) * 5; // 10 
    uint8_t uv4 = 5 * (51 - (blockType / 51)); // 255 

    verts.insert(verts.end(), {
        v0.x, v0.y, v0.z,  uv1, uv2, (uint8_t)(normal.x + normal.z) , 
        v1.x, v1.y, v1.z,  uv3, uv2, (uint8_t)(normal.x + normal.z) , 
        v2.x, v2.y, v2.z,  uv1, uv4, (uint8_t)(normal.x + normal.z) , 

        v1.x, v1.y, v1.z, uv3, uv2, (uint8_t)(normal.x + normal.z) , 
        v3.x, v3.y, v3.z,  uv3, uv4, (uint8_t)(normal.x + normal.z) , 
        v2.x, v2.y, v2.z,  uv1, uv4, (uint8_t)(normal.x + normal.z) , 
        });

   /* verts.insert(verts.end(), {
        v0.x, v0.y, v0.z,  0, 0,
        v1.x, v1.y, v1.z,  1, 0,
        v2.x, v2.y, v2.z,  0, 1,

        v1.x, v1.y, v1.z, 1, 0,
        v3.x, v3.y, v3.z,  1, 1,
        v2.x, v2.y, v2.z,  0, 1
         });*/
}

void Chunk::render(Lengine::GLSLProgram& shader) {
   



        GLuint texturelocation = shader.getUnifromLocation("chunkOffset");
        glUniform3f(texturelocation, _chunkPosition.x, _chunkPosition.y, _chunkPosition.z);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Lengine::ResourceManager::getTexture("Textures/atlas.png").id);

        GLuint texturelocation2 = shader.getUnifromLocation("textures");
        glUniform1i(texturelocation2, 0);


        glm::vec3 ambientLightColor = glm::vec3(1.0f, 1.0f, 1.0f); // White ambient light
        float ambientStrength = 0.5f;  // Adjust this to control brightness

        // Pass values to the shader
        GLuint texturelocation3 = shader.getUnifromLocation("ambientLightColor");
        GLuint texturelocation4 = shader.getUnifromLocation("ambientStrength");
        glUniform3fv(texturelocation3, 1, glm::value_ptr(ambientLightColor));
        glUniform1f(texturelocation4, ambientStrength);




        for (int i = 0; i < 6; i++) {
            glBindVertexArray(VAO[i]);
            glDrawArrays(GL_TRIANGLES, 0, _vertices[i].size() / 5);
        }
    
}
void Chunk::setAttribute() {
    if (!_needsChunkUpdate) {
        VAO.resize(6);
        VBO.resize(6);
        for (int i = 0; i < 6; i++) {
            glGenVertexArrays(1, &VAO[i]);
            glGenBuffers(1, &VBO[i]);

            glBindVertexArray(VAO[i]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
            glBufferData(GL_ARRAY_BUFFER, _vertices[i].size() * sizeof(uint8_t), _vertices[i].data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, 6 * sizeof(uint8_t), (void*)(0));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_TRUE, 6 * sizeof(uint8_t), (void*)(3 * sizeof(uint8_t)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, 6 * sizeof(uint8_t), (void*)(5 * sizeof(uint8_t)));

            glBindVertexArray(0);
           
        }
        
    }
   
}

void Chunk::destroy() {
    if (isChunkLoaded) return;
    for (int i = 0; i < 6; i++) {
        if (VAO[i]) glDeleteVertexArrays(1, &VAO[i]);
        if (VBO[i]) glDeleteBuffers(1, &VBO[i]);
    }
    VAO.clear();
    VBO.clear();
    _vertices.clear();

    isChunkLoaded = false;
}


void Chunk::saveChunk(const std::string& worldSavePath)
{
    std::string filepath = worldSavePath + "/" + std::to_string(_chunkPosition.x) + "_" + std::to_string(_chunkPosition.z) + ".bin";
    FILE* fp = nullptr;

    if (fopen_s(&fp, filepath.c_str(), "wb") != 0 || !fp) {
        std::cerr << "Error: Failed to open file for saving: " << filepath << std::endl;
        return;
    }

    // Save chunk position
    if (fwrite(&_chunkPosition.x, sizeof(int), 1, fp) != 1 ||
        fwrite(&_chunkPosition.z, sizeof(int), 1, fp) != 1) {
        std::cerr << "Error: Failed to write chunk position" << std::endl;
        fclose(fp);
        return;
    }

    // Save block data
    size_t blocksWritten = fwrite(_blocks, sizeof(int8_t), CHUNK_SIZE * CHUNK_SIZE_Y * CHUNK_SIZE, fp);
    if (blocksWritten != CHUNK_SIZE * CHUNK_SIZE_Y * CHUNK_SIZE) {
        std::cerr << "Error: Failed to write all block data" << std::endl;
    }

    fclose(fp);
}

void Chunk::loadChunk(const std::string& worldSavePath, int x, int z)
{
    if (isChunkLoaded) {
        std::string filepath = worldSavePath + "/" + std::to_string(x) + "_" + std::to_string(z) + ".bin";
        FILE* fp = nullptr;

        if (fopen_s(&fp, filepath.c_str(), "rb") != 0 || !fp) {
            std::cerr << "Error: Failed to open file for loading: " << filepath << std::endl;
            return;
        }

        // Read chunk position and update _chunkPosition
        if (fread(&_chunkPosition.x, sizeof(int), 1, fp) != 1 ||
            fread(&_chunkPosition.z, sizeof(int), 1, fp) != 1) {
            std::cerr << "Error: Failed to read chunk position" << std::endl;
            fclose(fp);
            return;
        }

        // Read block data
        size_t blocksRead = fread(_blocks, sizeof(int8_t), CHUNK_SIZE * CHUNK_SIZE_Y * CHUNK_SIZE, fp);
        if (blocksRead != CHUNK_SIZE * CHUNK_SIZE_Y * CHUNK_SIZE) {
            std::cerr << "Error: Failed to read all block data" << std::endl;
        }

        fclose(fp);
        isChunkLoaded = false;
    }
}


