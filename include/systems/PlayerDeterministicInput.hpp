#pragma once

#include "systems/System.hpp"

struct S_PlayerDeterministicInput : public System<S_PlayerDeterministicInput>
{
        void update(float deltaTime, entt::registry &reg);
};
