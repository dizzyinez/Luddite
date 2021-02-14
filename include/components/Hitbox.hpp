#pragma once
#include <glm/vec2.hpp>

struct C_Hitbox
{
        C_Hitbox() = default;
        C_Hitbox(float x, float y, float damage_) : kb_dir(glm::vec2(x, y)), damage(damage_) {}
        glm::vec2 kb_dir = glm::vec2(0.0f, 0.0f);
        float damage = 0;
        bool stun = true;
        bool enabled = false;
};