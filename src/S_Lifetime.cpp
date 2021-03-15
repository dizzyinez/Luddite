#include "systems/Lifetime.hpp"
#include "components/GameComponents.hpp"

void S_Lifetime::update(float deltaTime, entt::registry &reg)
{
    std::vector<entt::entity> to_del;
    for (auto &&[entity, lifetime]: reg.view<C_Lifetime>().each())
    {
        lifetime.timer--;
        if (lifetime.timer <= 0)
        {
            to_del.emplace_back(entity);
        }
    }
    for (auto e : to_del)
            reg.destroy(e);
}