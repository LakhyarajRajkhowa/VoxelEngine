#pragma once

#include <iostream>
#include <vector>
#include <stb_image.h>
#include <stb_image_write.h>


#include <stb_image_resize.h>  // Include the resizer

const int TILE_SIZE = 16;
const int ATLAS_ROWS = 51;
const int ATLAS_COLS = 51;
const int ATLAS_WIDTH = TILE_SIZE * ATLAS_COLS;
const int ATLAS_HEIGHT = TILE_SIZE * ATLAS_ROWS;

const char* textureFiles[] = {
    // grass with dirt = 1
    "Textures/grassTop.png",
    "Textures/grassSide.png",
    "Textures/dirtBlock.png",

    // dirt = 4
    "Textures/dirtBlock.png",
    "Textures/dirtBlock.png",
    "Textures/dirtBlock.png",

    //cobblestone = 7
    "Textures/stone.png",
    "Textures/stone.png",
    "Textures/stone.png",

    //stone = 10
     "Textures/MountainStone.png",
     "Textures/MountainStone.png",
     "Textures/MountainStone.png",

     // sand = 13
    "Textures/sand.png",
    "Textures/sand.png",
    "Textures/sand.png",

    // wood = 16
    "Textures/wood.png",
    "Textures/wood.png",
    "Textures/wood.png",

    // White wool = 19
    "Textures/whiteWool.png",
    "Textures/whiteWool.png",
    "Textures/whiteWool.png",

    // Black wool = 22
    "Textures/black_wool.png",
    "Textures/black_wool.png",
    "Textures/black_wool.png",

    // Red wool = 25
    "Textures/redWool.png",
    "Textures/redWool.png",
    "Textures/redWool.png",

    // Green wool = 28
    "Textures/greenWool.png",
    "Textures/greenWool.png",
    "Textures/greenWool.png",

    //Blue wool = 31
    "Textures/light_blue_wool.png",
    "Textures/light_blue_wool.png",
    "Textures/light_blue_wool.png"
 
};

const int sizeOfTextureFiles = sizeof(textureFiles) / sizeof(textureFiles[1]);

void createTextureAtlas() {
    
    std::vector<unsigned char> atlasData(ATLAS_WIDTH * ATLAS_HEIGHT * 4, 255);

    int index = 0;
    for (int row = 0; row < ATLAS_ROWS; row++) {
        for (int col = 0; col < ATLAS_COLS; col++) {
            if (index >= sizeOfTextureFiles) break;

            int w, h, channels;
            unsigned char* image = stbi_load(textureFiles[index], &w, &h, &channels, 4);
            if (!image) {
                std::cerr << "❌ Failed to load: " << textureFiles[index] << std::endl;
                continue;
            }

            std::cout << "✅ Loaded: " << textureFiles[index] << " (" << w << "x" << h << ")\n";

            unsigned char resizedImage[TILE_SIZE * TILE_SIZE * 4];

            if (w != TILE_SIZE || h != TILE_SIZE) {
                std::cout << "🔄 Resizing " << textureFiles[index] << " from " << w << "x" << h << " to 16x16\n";
                stbir_resize_uint8(image, w, h, 0, resizedImage, TILE_SIZE, TILE_SIZE, 0, 4);
            }
            else {
                memcpy(resizedImage, image, TILE_SIZE * TILE_SIZE * 4);
            }

            for (int y = 0; y < TILE_SIZE; y++) {
                for (int x = 0; x < TILE_SIZE; x++) {
                    int atlasX = col * TILE_SIZE + x;
                    int atlasY = row * TILE_SIZE + y;
                    int atlasIndex = (atlasY * ATLAS_WIDTH + atlasX) * 4;
                    int imageIndex = (y * TILE_SIZE + x) * 4;

                    for (int c = 0; c < 4; c++) {
                        atlasData[atlasIndex + c] = resizedImage[imageIndex + c];
                    }
                }
            }

            stbi_image_free(image);
            index++;
        }
    }

    if (!stbi_write_png("Textures/atlas.png", ATLAS_WIDTH, ATLAS_HEIGHT, 4, atlasData.data(), ATLAS_WIDTH * 4)) {
        std::cerr << "❌ Failed to save atlas.png\n";
    }
    else {
        std::cout << "✅ Texture Atlas saved as atlas.png\n";
    }
}


