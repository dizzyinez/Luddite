#pragma once
#include <string>
#include "glm/glm.hpp"
#include <map>
#include "data/Texture.hpp"
#include <memory>

struct Character
{
        // uint32_t texture_id;
        std::shared_ptr<Texture> texture;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        unsigned int Advance;
        // glm::vec2 tex_coords;
};

struct Font
{
        std::string name;
        std::map<char, Character> characters;
        Font(std::string name_)
                : name(name_)
        {}
};
