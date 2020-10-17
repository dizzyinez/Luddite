#pragma once
#include "data/ResourceAllocator.hpp"
#include "data/Texture.hpp"

class TextureAllocator : public ResourceAllocator<Texture>
{
        // std::shared_ptr<Texture> Allocate(std::string file_path);
};
