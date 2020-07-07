#ifndef Texture_hpp
#define Texture_hpp

#include "entityx/entityx.h"

#include <SDL2/SDL_image.h>
#include <string>
#include "Locator.hpp"
#include "TextureAllocator.hpp"
//#include <memory>

struct Texture : entityx::Component<Texture>
{
        int textureid;
        std::shared_ptr<SDL_Texture*> tex;
        SDL_Rect from;
        //rect of to and from
        Texture(std::string filepath)
        {
                TextureAllocator *TA = Locator::getTexureAllocator();
                textureid = TA->GetID(filepath);
                LoadTexture();
        }

        Texture(std::string filepath, int layer_)
        {
                TextureAllocator *TA = Locator::getTexureAllocator();
                textureid = TA->GetID(filepath);
                LoadTexture();
        }

        void LoadTexture()
        {
                tex = Locator::getTexureAllocator()->Get(textureid);
                int w, h;
                SDL_QueryTexture(*tex.get(), NULL, NULL, &w, &h);
                from.x = 0;
                from.y = 0;
                from.w = w;
                from.h = h;
        }

        int height()
        {
                return 0;
        }

        //get
};

#endif
