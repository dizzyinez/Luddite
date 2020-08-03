#include "systems/Motion.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"

#include "events/events.hpp"

#include <iostream>

void S_Motion::update(float deltaTime, entt::registry &reg)
{
        reg.group<C_Position, C_Velocity>().each([deltaTime](auto Entity, auto &pos, auto &vel){
                pos.position += vel.velocity * deltaTime;
        });
}
