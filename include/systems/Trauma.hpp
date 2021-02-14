#pragma once
#include "systems/System.hpp"

struct S_Trauma : public System<S_Trauma>
{
        void update(float deltaTime, entt::registry &reg);
};
