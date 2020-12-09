#pragma once
#include "systems/System.hpp"

struct S_AnimationBehavior : public System<S_AnimationBehavior>
{
        void update(float deltaTime, entt::registry &reg);
};
