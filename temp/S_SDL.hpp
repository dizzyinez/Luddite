#ifndef S_SDL_hpp
#define S_SDL_hpp


#include "entityx/entityx.h"
#include <SDL2/SDL.h>

struct S_SDL : entityx::component<S_SDL>
{
        SDL_Window* window;
        SDL_Renderer* renderer;
        S_SDL(SDL_Window* window_, SDL_Renderer* renderer_) : window(window_), renderer(renderer_) {
        }
};

#endif
