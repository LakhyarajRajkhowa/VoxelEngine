#include <Windows.h>

#define SDL_MAIN_HANDLED
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define ZLIB_STATIC

#include "MainGame.h"
#include "TextureAtlas.h"
#include <iostream>
#include <Lengine/SimplexNoise.h>



bool createFile(const char* filePath) {
    DWORD fileAttrib = GetFileAttributesA(filePath);
    if (fileAttrib = INVALID_FILE_ATTRIBUTES) {
        if (!(CreateDirectoryA(filePath, NULL))) {
            std::cout << "FAILED//" << std::endl;
            return false;
        }
    }
    return true;
}


int main(int argc, char* argv[]) {
    
   // createTextureAtlas();
    MainGame mainGame;
    mainGame.run();
    
    
    
    return 0;

}



