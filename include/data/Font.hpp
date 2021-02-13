#pragma once
#include <string>
#include "core/basic_includes.hpp"
#include "glm/glm.hpp"
#include "data/Texture.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "data/ResourceAllocator.hpp"
#include <map>
#include <vector>
#include <memory>

enum class FontFamily : uint8_t
{
        HEADING,
        BODY
};

struct Character
{
        // uint32_t texture_id;
        std::shared_ptr<Texture> texture;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        long int Advance;
        bool missing;
        bool colored;
        // glm::vec2 tex_coords;
};

struct Font
{
        Font(const std::string& file_path);
        // std::map<, Character> characters;
        // std::unordered_map<uint32_t, Character> characters;
        static inline FT_Library ft;
        FT_Face face;
        ResourceAllocator_not_static<Character, char32_t> characters;
        std::shared_ptr<Character> RenderCharacter(char32_t charcode);
        bool HasCharacter(char32_t charcode);
        std::shared_ptr<Character> GetCharacter(char32_t charcode);
        static void initialize_fonts();
        static inline std::map<FontFamily, std::vector<std::shared_ptr<Font> > > font_families;
        static std::vector<std::shared_ptr<Font> >& GetFamily(FontFamily family);
};
