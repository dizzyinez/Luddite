#ifndef TextureAllocator_hpp
#define TextureAllocator_hpp

#include "ResourceAllocator.hpp"
#include <SDL2/SDL_image.h>

class TextureAllocator : public ResourceAllocator<SDL_Texture*>
{
public:
TextureAllocator(/*SDL_Renderer* renderer_) : renderer(renderer_*/)
{
}
std::shared_ptr<SDL_Texture*> getResource(const std::string& filepath)
{
        SDL_Surface* tempSurface = IMG_Load(filepath.c_str());
        if (!tempSurface) {
                std::cout << "IMG_Load failed: " << IMG_GetError() << std::endl;
                //tempSurface = IMG_Load("Assets/missing texture.png");
                SDL_FreeSurface(tempSurface);
                return nullptr;
        }
        std::shared_ptr<SDL_Texture*> tex = std::make_shared<SDL_Texture*>(SDL_CreateTextureFromSurface(renderer, tempSurface));
        SDL_FreeSurface(tempSurface);

        return tex;
}
void removeResource(std::shared_ptr<SDL_Texture*> resource)
{
        SDL_DestroyTexture(*resource);
}

void setRenderer(SDL_Renderer* renderer_)
{
        renderer = renderer_;
}

void setWindow(SDL_Window* window_)
{
        window = window_;
}

void RenderCopy(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dest)
{
        //TODO: check bounds
        SDL_RenderCopy(renderer, tex, src, dest);
}


void setWindowIcon(int id)
{
        std::string filepath = getFilePath(id);
        if (filepath != "")
        {

                SDL_Surface* tempSurface = IMG_Load(filepath.c_str());
                if (!tempSurface) {
                        std::cout << "IMG_Load failed: " << IMG_GetError() << std::endl;
                        //tempSurface = IMG_Load("Assets/missing texture.png");
                        SDL_FreeSurface(tempSurface);
                        return;
                }
                SDL_SetWindowIcon(window, tempSurface);
                SDL_FreeSurface(tempSurface);
        }
}

void setWindowIcon(std::string filepath)
{
        SDL_Surface* tempSurface = IMG_Load(filepath.c_str());
        if (!tempSurface) {
                std::cout << "IMG_Load failed: " << IMG_GetError() << std::endl;
                //tempSurface = IMG_Load("Assets/missing texture.png");
                SDL_FreeSurface(tempSurface);
                return;
        }
        SDL_SetWindowIcon(window, tempSurface);
        SDL_FreeSurface(tempSurface);
}

private:
SDL_Renderer* renderer;
SDL_Window* window;
};

#endif
