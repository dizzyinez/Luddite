#ifndef C_Texture_hpp
#define C_Texture_hpp
#include "data/Texture.hpp"
#include <glm/glm.hpp>

struct C_Texture
{
        C_Texture() = default;
        C_Texture(std::shared_ptr<Texture> texture_)
                : texture(texture_)
        {
        }
        C_Texture(std::shared_ptr<Texture> texture_, const glm::vec4& tex_coords_)
                : texture(texture_), tex_coords(tex_coords_)
        {
        }
        std::shared_ptr<Texture> texture;
        glm::vec4 tex_coords = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
};

struct C_Sprite
{
        C_Sprite() = default;
        C_Sprite(const glm::uvec4& colors_)
                : colors(colors_)
        {
        }
        C_Sprite(std::shared_ptr<Texture> texture_, const glm::uvec4& colors_)
                : texture(texture_), colors(colors_)
        {
        }
        C_Sprite(std::shared_ptr<Texture> texture_, const glm::vec4& tex_coords_)
                : texture(texture_), tex_coords(tex_coords_)
        {
        }
        std::shared_ptr<Texture> texture;
        glm::vec4 tex_coords = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        glm::uvec4 colors = glm::uvec4(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
};

#endif
