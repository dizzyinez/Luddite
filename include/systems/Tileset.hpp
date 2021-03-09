#pragma once
#include "systems/System.hpp"

struct S_Tilemap : public System<S_Tilemap>
{
        void update(float deltaTime, entt::registry &reg);
};
