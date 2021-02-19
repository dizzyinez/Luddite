#pragma once
#include <stdint.h>
#include <array>
#include <vector>
#include <glm/glm.hpp>

struct LevelSection
{
        LevelSection(const glm::vec2& center_, uint8_t type_) : center(center_), type(type_) {}
        glm::vec2 center;
        uint8_t type;
        std::vector<glm::vec2> voronoi_boundary;
        std::vector<uint32_t> neighbor_section_indicies;
};

struct Level
{
        std::vector<LevelSection> level_sections;
};

struct LevelGenerator
{
        static Level Generate(uint32_t seed);
};