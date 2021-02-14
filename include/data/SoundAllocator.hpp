#pragma once
#include "data/ResourceAllocator.hpp"
#include "data/Sound.hpp"

class SoundAllocator : public ResourceAllocator<Sound>
{
        // std::shared_ptr<Texture> Allocate(std::string file_path);
};
