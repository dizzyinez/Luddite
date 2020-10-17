#pragma once
#include "data/ResourceAllocator.hpp"
#include "data/Json.hpp"

class JsonAllocator : public ResourceAllocator<Json>
{
        // std::shared_ptr<Json> Allocate(std::string file_path);
};
