#pragma once

#include "systems/System.hpp"

struct S_PlayerDirection : public System<S_PlayerDirection>
{
        void update(float deltaTime, entt::registry &reg) override;
};
