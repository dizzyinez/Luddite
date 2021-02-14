#pragma once

#include "systems/System.hpp"

struct S_Camera : public System<S_Camera>
{
        void update(float alpha, float deltaTime, entt::registry& lerp_reg, entt::registry& reg);
};