#pragma once

#include "systems/System.hpp"

struct S_DrawLevel : public System<S_DrawLevel>
{
        void update(float deltaTime, entt::registry &reg);
};
