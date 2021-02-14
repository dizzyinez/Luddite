#include "data/TextureAllocator.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

template <>
std::shared_ptr<Texture> ResourceAllocator<Texture>::Allocate(std::string file_path)
{
        uint32_t texture;
        stbi_set_flip_vertically_on_load(false);


        int width, height, nrChannels;
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 4);
        if (data == nullptr)
                std::cout << "unable to load file: " << stbi_failure_reason() << " | filepath: " << file_path << std::endl;
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

        return std::make_shared<Texture>(texture, width, height);
}
