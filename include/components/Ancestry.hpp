#pragma once
#include "entt.hpp"

struct C_Child
{
        C_Child(entt::entity parent_)
                : parent(parent_)
        {}
        entt::entity parent;
        glm::vec3 offset{0.0f, 0.0f, 0.0f};
};