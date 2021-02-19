#include "systems/Ancestry.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Ancestry.hpp"

void S_Ancestry::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_Child>().each([&reg](auto Entity, auto &child) {
                C_Position& pos = reg.get<C_Position>(Entity);
                pos.position = reg.get<C_Position>(child.parent).position + child.offset;
        });
}