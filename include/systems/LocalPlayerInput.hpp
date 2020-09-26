#pragma once
#include "systems/System.hpp"

struct S_LocalPlayerInput : public System<S_LocalPlayerInput>
{
        void update(float deltaTime, entt::registry &reg) override;
};
