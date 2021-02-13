#include "data/Font.hpp"
#include "data/FontAllocator.hpp"
#include <freetype/ftimage.h>

Font::Font(const std::string& file_path)
{
        if (FT_New_Face(ft, file_path.c_str(), 0, &face))
        {
                std::cout << "Unable to load font from " << file_path << std::endl;
                return;
        }
        FT_Select_Charmap(face, ft_encoding_unicode);
        FT_Set_Pixel_Sizes(face, 0, 128);


        // String to_pre_render(U"ADBS");
        String to_pre_render(U" ABCČDEFGHIJKLMNOPQRSŠTUVWXYZŽabcčdefghijklmnopqrsštuvwxyzž1234567890'‘?’“!”(%)[#]{@}/&\\<-+÷×=>®©$€£¥¢:;,.*_");
        for (char32_t c : to_pre_render)
        {
                std::shared_ptr<Character> character = RenderCharacter(c);
                characters.AddFakeUser(c, character);
        }
}

bool Font::HasCharacter(char32_t charcode)
{
        return characters.Has(charcode);
}

std::shared_ptr<Character> Font::GetCharacter(char32_t charcode)
{
        return characters.Get(charcode);
}

std::shared_ptr<Character> Font::RenderCharacter(char32_t charcode)
{
        FT_UInt glyph_index = FT_Get_Char_Index(face, charcode);
        // load  glyph
        FT_Int32 flags = FT_LOAD_FORCE_AUTOHINT | FT_LOAD_DEFAULT;
        bool has_color = false;
        if (FT_HAS_COLOR(face))
        {
                has_color = true;
                flags |= FT_LOAD_COLOR;
        }
        else
                flags |= FT_LOAD_RENDER;
        if (FT_Load_Glyph(face, glyph_index, flags))
        {
                std::cout << "FreeType: Failed to load Glyph" << std::endl;
                return nullptr;
        }
        //generate texture
        uint32_t texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        if (has_color)
        {
                // GLint swizzle_mask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
                // glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
                if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
                {
                        std::cout << "FreeType: Failed to render Glyph" << std::endl;
                        return nullptr;
                }
                if (face->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_BGRA)
                {
                        uint32_t color = 0xFF00FFFF;
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
                }
                else
                {
                        glTexImage2D(
                                GL_TEXTURE_2D,
                                0,
                                GL_RGBA,
                                face->glyph->bitmap.width,
                                face->glyph->bitmap.rows,
                                0,
                                GL_BGRA,
                                GL_UNSIGNED_BYTE,
                                face->glyph->bitmap.buffer
                                );
                }
        }
        else
        {
                GLint swizzle_mask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
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
        }


        auto character = std::make_shared<Character>(Character{
                std::make_shared<Texture>(texture, face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x,
                glyph_index == 0,
                has_color
        });
        characters.Add(charcode, character);
        return character;
}

void Font::initialize_fonts()
{
        font_families.insert(std::make_pair(FontFamily::HEADING, std::vector<std::shared_ptr<Font> >{
                FontAllocator::Get("../assets/fonts/OpenDyslexic.otf"),
                FontAllocator::Get("../assets/fonts/ConcertOne-Regular.ttf")
        }));
        font_families.insert(std::make_pair(FontFamily::BODY, std::vector<std::shared_ptr<Font> >{
                // FontAllocator::Get("../assets/fonts/OpenDyslexic.otf"),
                FontAllocator::Get("../assets/fonts/Ubuntu-Regular.ttf"),
                FontAllocator::Get("../assets/fonts/ConcertOne-Regular.ttf"),
                FontAllocator::Get("../assets/fonts/seguiemj.ttf")
        }));
        // std::vector<std::shared_ptr<Font> > fonts;
        // fonts.push_back(FontAllocator::Get("../assets/fonts/OpenDyslexic.otf"));
        // fonts.push_back(FontAllocator::Get("../assets/fonts/ConcertOne-Regular.ttf"));
        // font_families.insert(std::make_pair(FontFamily::BODY, fonts));
}

std::vector<std::shared_ptr<Font> >& Font::GetFamily(FontFamily family)
{
        return font_families.find(family)->second;
}

template <>
std::shared_ptr<Character> ResourceAllocator_not_static<Character, char32_t>::Allocate(char32_t character)
{
        return nullptr;
}
