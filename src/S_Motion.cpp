
#include "systems/Motion.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Ancestry.hpp"
#include "components/Drag.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

#include "events/events.hpp"

#include <iostream>

void S_Motion::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_Child>().each([&reg](auto Entity, auto &child) {
                C_Position& pos = reg.get<C_Position>(Entity);
                pos.position = reg.get<C_Position>(child.parent).position + child.offset;
        });
        reg.group<C_Position, C_Velocity>().each([deltaTime](auto Entity, auto &pos, auto &vel) {
                pos.position += vel.velocity * deltaTime;
        });
        // reg.group<C_Position, C_Velocity, C_Drag>().each([deltaTime](auto Entity, auto &pos, auto &vel, auto &drag) {
        //         vel.velocity *= glm::pow(drag, deltaTime); //pretty expensive, try to avoid using the drag component
        // });
}
