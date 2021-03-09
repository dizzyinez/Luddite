#pragma once
#include "data/TilemapAllocator.hpp"
struct C_Tilemap
{
        C_Tilemap(std::shared_ptr<Tilemap> tilemap_) : tilemap(tilemap_) {}
        std::shared_ptr<Tilemap> tilemap;
        uint32_t index;
};
