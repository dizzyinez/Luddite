#include "data/Tilemap.hpp"
#include <fstream>

Tilemap::Tilemap(const std::string& file_path)
{
        std::ifstream tilemap_file(file_path.c_str(), std::ios::binary);
        if (tilemap_file.is_open())
        {
                tilemap_file.seekg(0, std::ios::end);
                std::size_t size = tilemap_file.tellg();
                buffer.resize(size);
                tilemap_file.seekg(0, std::ios::beg);
                tilemap_file.read(&buffer[0], size);
                tilemap_file.close();
        }
}
uint16_t Tilemap::get_x(int index) {return *(uint16_t*)&buffer[index * 6 * 2 + 0];}
uint16_t Tilemap::get_y(int index) {return *(uint16_t*)&buffer[index * 6 * 2 + 2];}
uint16_t Tilemap::get_w(int index) {return *(uint16_t*)&buffer[index * 6 * 2 + 4];}
uint16_t Tilemap::get_h(int index) {return *(uint16_t*)&buffer[index * 6 * 2 + 6];}
uint16_t Tilemap::get_origin_x(int index) {return *(uint16_t*)&buffer[index * 6 * 2 + 8];}
uint16_t Tilemap::get_origin_y(int index) {return *(uint16_t*)&buffer[index * 6 * 2 + 10];}