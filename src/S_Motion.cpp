#include "systems/Motion.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Drag.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

#include "events/events.hpp"

#include <iostream>

void S_Motion::update(float deltaTime, entt::registry &reg)
{
        reg.group<C_Position, C_Velocity>().each([deltaTime](auto Entity, auto &pos, auto &vel){
                pos.position += vel.velocity * deltaTime;
        });
        reg.group<C_Position, C_Velocity, C_Drag>().each([deltaTime](auto Entity, auto &pos, auto &vel, auto &drag){
                vel.velocity *= glm::pow(drag, deltaTime); //pretty expensive, try to avoid using the drag component
        });
        reg.group<>(entt::get<C_Position, C_Net_Position>).each([deltaTime](auto Entity, auto &pos, auto &net_pos){
                if (deltaTime < net_pos.time)
                {
                        pos.position = glm::lerp(pos.position, net_pos.position, (float)(deltaTime/net_pos.time));
                        net_pos.time -= deltaTime;
                }
                else
                {
                        //more time has elapsed than should take for the remaining lerping
                        pos.position = net_pos.position;
                        net_pos.time = 0;
                }
        });
}
