#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <Lengine/Lengine.h>

namespace Lengine {

    int init() {

        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        return 0;
    }
}