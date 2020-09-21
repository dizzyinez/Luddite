#pragma once
#include "systems/System.hpp"

struct S_Tileset : public System<S_Tileset>
{
        void update(float deltaTime, entt::registry &reg) override;
};
