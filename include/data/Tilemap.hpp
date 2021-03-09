#pragma once
#include <stdint.h>
#include <string>
#include <vector>

struct Tilemap
{
        Tilemap(const std::string& file_path);
        std::vector<char> buffer;
        uint16_t get_x(int index);
        uint16_t get_y(int index);
        uint16_t get_w(int index);
        uint16_t get_h(int index);
        uint16_t get_origin_x(int index);
        uint16_t get_origin_y(int index);
};