#pragma once
#include "systems/System.hpp"

struct S_Transform_Lerp : public System<S_Transform_Lerp>
{
        void update(entt::registry& before_reg, entt::registry& after_reg, entt::registry& lerp_reg, float alpha);
};