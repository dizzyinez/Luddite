#include "systems/PlayerController.hpp"
#include "components/Player.hpp"
#include "components/PlayerKeymap.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/AnimationBehavior.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"
#include <GLFW/glfw3.h>

#include <glm/gtx/string_cast.hpp>

constexpr float BASE_MOVE_SPEED = 6.f;
void S_PlayerController::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_Player>().each([&reg](auto Entity, auto &player) {
                auto &&[input, d_input, vel, abs] = reg.get<C_PlayerInput, C_PlayerDeterministicInput, C_Velocity, C_AnimationBehaviorState>(Entity);

                //dash
                if (d_input.button3_release_age <= 8 && player.state.dash_timer == 0)
                {
                        player.state.dash_timer = 10;
                }


                //set the movement doirection of the player based on input.
                float speed;
                if (abs.motion_lock)
                        speed = 0;
                else
                        speed = BASE_MOVE_SPEED * abs.movement_speed;
                glm::vec2 dir = input.direction();
                vel.velocity.x = (dir.x * speed) + abs.anim_vel_x;
                vel.velocity.y = (dir.y * speed) + abs.anim_vel_y;
                //set the integer direction of the player
                auto anim_dir = input.animation_direction();
                if (anim_dir >= 0)
                        d_input.movement_direction = anim_dir;
        });
}
