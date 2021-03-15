#include "systems/PlayerController.hpp"
#include "components/GameComponents.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"
#include <GLFW/glfw3.h>

#include <glm/gtx/string_cast.hpp>

constexpr float BASE_MOVE_SPEED = 6.f;
void S_PlayerController::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_Player>().each([&reg](auto entity, auto &player) {
                auto &&[input, d_input, vel, abs] = reg.get<C_PlayerInput, C_PlayerDeterministicInput, C_Velocity, C_AnimationBehaviorState>(entity);



                //dash
                if (d_input.button4_release_age <= 8 && player.state.dash_timer == 0)
                {
                        player.state.dash_timer = 10 + 1;
                }
                glm::vec2 dash_dir = glm::vec2(0);
                if (player.state.dash_timer > 0)
                {
                        player.state.dash_timer--;
                        dash_dir = input.direction() * 12.f;

                        //create ghosts
                        if (player.state.dash_timer % 2 == 0)
                        {
                                auto ghost = reg.create();
                                reg.emplace<C_Position>(ghost, reg.get<C_Position>(entity));
                                reg.emplace<C_Size>(ghost, reg.get<C_Size>(entity));
                                reg.emplace<C_Sprite>(ghost, reg.get<C_Sprite>(entity)).colors.x = 0;
                                reg.emplace<C_DrawLayer>(ghost, reg.get<C_DrawLayer>(entity));
                                reg.emplace<C_Tilemap>(ghost, reg.get<C_Tilemap>(entity));
                                reg.emplace<C_Lifetime>(ghost, 10);
                        }
                }


                //set the movement direction of the player based on input.
                float speed;
                if (abs.motion_lock)
                        speed = 0;
                else
                        speed = BASE_MOVE_SPEED * abs.movement_speed;
                glm::vec2 move_dir = input.direction() * speed;

                vel.velocity = glm::vec3(move_dir + dash_dir + glm::vec2(abs.anim_vel_x, abs.anim_vel_y), 0.f);
                //set the integer direction of the player
                auto anim_dir = input.animation_direction();
                if (anim_dir >= 0)
                        d_input.movement_direction = anim_dir;
        });
}
