#include "GameElements.h"
#include <iostream>
#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>


void GameElements::drawCrosshair() {
   // glDisable(GL_DEPTH_TEST); // Disable depth test so it always appears on top
    glBegin(GL_LINES);

    // Set white color
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

    // Horizontal line
    glVertex2f(-0.01f , 0.0f);
    glVertex2f(0.01f , 0.0f);

    // Vertical line
    glVertex2f(0.0f , -0.0175f);
    glVertex2f(0.0f , 0.0175f);

    glEnd();
  //  glEnable(GL_DEPTH_TEST); // Re-enable depth test

}




    GameElements::RayHit GameElements::castRay(const glm::vec3 & origin, const glm::vec3 & direction, int maxDistance, std::unordered_map<int, std::unordered_map<int, std::unique_ptr<Chunk>>>&chunk) {
        glm::vec3 pos = origin;
        glm::vec3 lastBlockPos = pos;

        for (int i = 0; i < maxDistance * 1500; i++) {

            glm::vec3 blockPos = pos;
           
            

            // If the block is solid
            if (blockPos.x >= 0 && blockPos.y >= 0 && blockPos.z >= 0)
                if (chunk[blockPos.x / 16][blockPos.z / 16]->getBlock((int)blockPos.x % 16, (int)blockPos.y % 127, (int)blockPos.z % 16) != 0) {
                   
                   // std::cout << (int)lastBlockPos.x  << " " << (int)lastBlockPos.y  << " " << (int)lastBlockPos.z  << std::endl;
                    
                    blockPos = glm::ivec3( (int)blockPos.x , (int)blockPos.y , (int)blockPos.z  );
                    lastBlockPos = glm::ivec3((int)lastBlockPos.x , (int)lastBlockPos.y , (int)lastBlockPos.z );

                    return { true, blockPos, lastBlockPos };
                }
            lastBlockPos = glm::floor(pos);
            pos += direction * 0.01f;

        }


        return { false, {}, {} };
    }


