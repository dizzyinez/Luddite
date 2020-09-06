#ifndef C_Texture_hpp
#define C_Texture_hpp

struct C_Texture
{
        C_Texture() = default;
        C_Texture(uint32_t texture_id_)
                : texture_id(texture_id_)
        {
        }
        C_Texture(uint32_t texture_id_, const glm::vec4& tex_coords_)
                : texture_id(texture_id_), tex_coords(tex_coords_)
        {
        }
        uint32_t texture_id;
        glm::vec4 tex_coords = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
};

struct C_Sprite
{
        C_Sprite() = default;
        C_Sprite(const glm::uvec4& colors_)
                : colors(colors_)
        {
        }
        C_Sprite(uint32_t texture_id_, const glm::uvec4& colors_)
                : texture_id(texture_id_), colors(colors_)
        {
        }
        C_Sprite(uint32_t texture_id_, const glm::vec4& tex_coords_)
                : texture_id(texture_id_), tex_coords(tex_coords_)
        {
        }
        uint32_t texture_id;
        glm::vec4 tex_coords = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        glm::uvec4 colors = glm::uvec4(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
};

#endif
