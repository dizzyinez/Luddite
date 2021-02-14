#include "systems/Trauma.hpp"
#include "components/GameComponents.hpp"

constexpr float TRAUMA_DECAY_COEFFICIENT = 1.0f;
void S_Trauma::update(float deltaTime, entt::registry &reg)
{
        for (auto &&[Entity, player] : reg.view<C_Player>().each())
        {
                player.trauma -= TRAUMA_DECAY_COEFFICIENT * deltaTime;
                if (player.trauma > 1.f)
                        player.trauma = 1.f;
                if (player.trauma < 0.f)
                        player.trauma = 0.f;
        }
}