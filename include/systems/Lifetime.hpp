#pragma once

#include "systems/System.hpp"

struct S_Lifetime : public System<S_Lifetime>
{
        void update(float deltaTime, entt::registry &reg);
};
