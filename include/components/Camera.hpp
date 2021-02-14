#pragma once

#include <glm/glm.hpp>

struct C_Camera
{
        glm::vec2 position = glm::vec2(0.f, 0.f);
        float noise_timer;
        float scale_coefficient = 1.f;
        float trauma_coefficient = 1.f;
};