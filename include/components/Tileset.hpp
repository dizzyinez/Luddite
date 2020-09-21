#pragma once
struct C_Tileset
{
        C_Tileset(uint16_t tiles_width_, uint16_t tiles_height_, uint32_t index_)
                : tiles_width(tiles_width_), tiles_height(tiles_height_), index(index_)
        {
        }
        uint16_t tiles_width;
        uint16_t tiles_height;
        uint32_t index;
};
