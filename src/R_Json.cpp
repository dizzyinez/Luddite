#include "data/JsonAllocator.hpp"

template <>
std::shared_ptr<Json> ResourceAllocator<Json>::Allocate(std::string file_path)
{
        return std::make_shared<Json>(file_path);
}
