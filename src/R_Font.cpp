#include "data/FontAllocator.hpp"
#include "data/Texture.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static FT_Library ft;
void FontAllocator::Initialize()
{
        if (FT_Init_FreeType(&ft))
        {
                std::cout << "Failed to Init FreeType Lib" << std::endl;
        }
}

template <>
std::shared_ptr<Font> ResourceAllocator<Font>::Allocate(std::string file_path)
{
        Font font(file_path); //should set the name to the actual name not the path
        FT_Face face;
        if (FT_New_Face(ft, file_path.c_str(), 0, &face))
        {
                std::cout << "Unable to load font from " << file_path << std::endl;
                return nullptr;
        }
        FT_Set_Pixel_Sizes(face, 0, 128);

        uint32_t textures[128];
        //generate textures
        glCreateTextures(GL_TEXTURE_2D, 100, textures);
        for (int c = 0; c < 128; c++)
        {
                uint32_t& texture = textures[c];
                //load character glyph
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                        std::cout << "FreeType: Failed to load Glyph" << std::endl;
                        continue;
                }
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                GLint swizzle_mask[] = {GL_ZERO, GL_ZERO, GL_ZERO, GL_RED};
                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
                glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RGBA,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer
                        );
                glBindTexture(GL_TEXTURE_2D, 0);


                // Character character = {
                //         Texture(2, face->glyph->bitmap.width, face->glyph->bitmap.rows),
                //         glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                //         glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                //         face->glyph->advance.x
                // };
                // Texture tex(texture, face->glyph->bitmap.width, face->glyph->bitmap.rows);
                Character character = {
                        // texture,
                        // tex,

                        std::make_shared<Texture>(texture, face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        // Texture(3, 2, 2),
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        face->glyph->advance.x
                };
                font.characters.insert(std::make_pair(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        return std::make_shared<Font>(font);
}
