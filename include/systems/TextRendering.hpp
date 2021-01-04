#pragma once

#include "systems/System.hpp"

struct S_Text_Rendering : public System<S_Text_Rendering>
{
        void update(float deltaTime, entt::registry &reg);
};