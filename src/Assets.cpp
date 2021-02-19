#include "data/Assets.hpp"

// #define STB_IMAGE_IMPLEMENTATION
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include "stb/stb_image.h"

// uint32_t Assets::Sprite::Get(AssetID id)
// {
//         // if (texture_slots[id] == 0)
//         // {
//         //         //init
//         //         glEnable(GL_BLEND);
//         //         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//         //         stbi_set_flip_vertically_on_load(false);
//         //
//         //
//         //         int width, height, nrChannels;
//         //         glCreateTextures(GL_TEXTURE_2D, 1, &Assets::Sprite::texture_slots[id]);
//         //         glBindTexture(GL_TEXTURE_2D, Assets::Sprite::texture_slots[id]);
//         //         unsigned char *data = stbi_load(Assets::Sprite::Data[id].file_path.c_str(), &width, &height, &nrChannels, 0);
//         //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//         //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         //         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//         //         glBindTexture(GL_TEXTURE_2D, 0);
//         //         stbi_image_free(data);
//         // }
//         return texture_slots[id];
// }
