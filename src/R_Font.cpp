#include "data/FontAllocator.hpp"
#include "data/Texture.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void FontAllocator::Initialize()
{
        if (FT_Init_FreeType(&Font::ft))
        {
                std::cout << "Failed to Init FreeType Lib" << std::endl;
        }
        Font::initialize_fonts();
}

template <>
std::shared_ptr<Font> ResourceAllocator<Font>::Allocate(std::string file_path)
{
        std::cout << "rendering font at path: " << file_path << std::endl;
        // Font new_font(file_path);
        return std::make_shared<Font>(file_path);
}
