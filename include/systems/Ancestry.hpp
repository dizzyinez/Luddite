#pragma once
#include "systems/System.hpp"

struct S_Ancestry : public System<S_Ancestry>
{
        void update(float deltaTime, entt::registry &reg);
};