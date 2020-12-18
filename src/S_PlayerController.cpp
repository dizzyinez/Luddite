#include "systems/PlayerController.hpp"
#include "components/Player.hpp"
#include "components/PlayerKeymap.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"
#include <GLFW/glfw3.h>

#include <glm/gtx/string_cast.hpp>
void S_PlayerController::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_Player>().each([&reg](auto Entity, auto &player) {
                auto [input, vel] = reg.get<C_PlayerInput, C_Velocity>(Entity);
                //set the movement direction of the player based on input.
                glm::vec2 dir = input.direction();
                vel.velocity.x = dir.x * 450;
                vel.velocity.y = dir.y * 450;


                //set the integer direction of the player
                auto anim_dir = input.animation_direction();
                if (anim_dir != -1)
                        input.movement_direction = anim_dir;
        });
}
