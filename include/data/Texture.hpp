#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

struct Texture
{
        unsigned int texture_id;
        Texture(uint32_t texture_id_, int width_, int height_)
                : texture_id(texture_id_), width(width_), height(height_)
        {
        }
        ~Texture();

        // int height()
        // {
        //         return 0;
        // }

        // operator const uint32_t&()
        // {
        //   return texture_id;
        // }
        int width;
        int height;
};
