#pragma once
#include "systems/System.hpp"

struct S_CharacterAgnosticAbilities : public System<S_CharacterAgnosticAbilities>
{
        void update(float deltaTime, entt::registry &reg);
};