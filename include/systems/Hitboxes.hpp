#pragma once

#include "systems/System.hpp"

struct S_Hitboxes : public System<S_Hitboxes>
{
        void update(float deltaTime, entt::registry &reg);
};
