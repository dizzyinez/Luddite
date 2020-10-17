#pragma once
#include "data/ResourceAllocator.hpp"
#include "data/Font.hpp"

class FontAllocator : public ResourceAllocator<Font>
{
public:
        static void Initialize();
        // std::shared_ptr<Font> Allocate(std::string file_path);
};
