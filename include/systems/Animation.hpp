#pragma once
#include "systems/System.hpp"

struct S_Animation : public System<S_Animation>
{
        void update(float deltaTime, entt::registry &reg) override;
};
