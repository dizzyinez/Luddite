#pragma once
#include <glm/vec2.hpp>

struct C_Origin
{
        C_Origin() = default;
        C_Origin(glm::vec2 origin_) : origin(origin_) {}
        C_Origin(float x, float y) : origin(x, y) {}
        glm::vec2 origin = glm::vec2(0.0f, 0.0f);
};