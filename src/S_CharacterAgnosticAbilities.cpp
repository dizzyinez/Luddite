#include "systems/CharacterAgnosticAbilities.hpp"
#include "components/GameComponents.hpp"

void S_CharacterAgnosticAbilities::update(float deltaTime, entt::registry &reg)
{
        for (auto &&[entity, player, d_input] : reg.group<C_Player, C_PlayerDeterministicInput>().each())
        {
                if (d_input.button3_release_age == 0)
                {
                }
        }
}