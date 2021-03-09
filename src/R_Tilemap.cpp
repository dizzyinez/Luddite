#include "data/TilemapAllocator.hpp"


template <>
std::shared_ptr<Tilemap> ResourceAllocator<Tilemap>::Allocate(std::string file_path)
{
        return std::make_shared<Tilemap>(file_path);
}