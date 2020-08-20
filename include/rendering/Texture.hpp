#ifndef Texture_hpp
#define Texture_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Locator.hpp"
//#include <memory>
#include "stb/stb_image.h"

struct Texture
{
        uint32_t texture_gl_id;
        Texture()
        {
                // TextureAllocator *TA = Locator::getTexureAllocator();
                // textureid = TA->GetID(filepath);
                // LoadTexture();
                int width, height, nrChannels;
                glCreateTextures(GL_TEXTURE_2D, 1, &texture_gl_id);
                glBindTexture(GL_TEXTURE_2D, texture_gl_id);
                unsigned char *data = stbi_load("../assets/textures/pallete.png", &width, &height, &nrChannels, 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(data);
        }

        void LoadTexture(std::string filepath)
        {



                // tex = Locator::getTexureAllocator()->Get(textureid);
                // int w, h;
                // SDL_QueryTexture(*tex.get(), NULL, NULL, &w, &h);
                // from.x = 0;
                // from.y = 0;
                // from.w = w;
                // from.h = h;
        }

        int height()
        {
                return 0;
        }

        //get
};

#endif
