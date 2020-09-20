#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Locator.hpp"
//#include <memory>
#include "stb/stb_image.h"

struct Texture
{
        uint32_t texture_id;
        Texture(const uint32_t texture_id_)
          :texture_id(texture_id_)
        {
        }
        ~Texture();

        // int height()
        // {
        //         return 0;
        // }

        operator const uint32_t&() {return texture_id;}
};
