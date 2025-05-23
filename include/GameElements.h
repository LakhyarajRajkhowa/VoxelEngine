#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include "Chunk.h"

class GameElements
{
public:
	void drawCrosshair();

    struct RayHit {
        bool hit;
        glm::ivec3 blockPos;
        glm::ivec3 adjacentPos;
    };
    GameElements::RayHit castRay(const glm::vec3& origin, const glm::vec3& direction, int maxDistance, std::unordered_map<int, std::unordered_map<int, std::unique_ptr<Chunk>>> &chunk);
 
};

