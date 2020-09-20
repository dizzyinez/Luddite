#include "systems/PlayerController.hpp"
#include "components/Player.hpp"
#include "components/PlayerKeymap.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"
#include <GLFW/glfw3.h>


void S_PlayerController::update(float deltaTime, entt::registry &reg)
{
        reg.group<C_Player>(entt::get<C_Position, C_Velocity, C_PlayerKeymap, C_PlayerDirection>).each([&reg](auto Entity, auto &player, auto &pos, auto &vel, auto &keymap, auto &direction){
                if (player.local_player)
                {
                        //set the movement direction of the player based on input.
                        Events::iterate<E_Keyboard>([&Entity, &keymap](auto &e){
                                if (e->action == GLFW_PRESS)
                                {
                                        if(e->key == keymap.move_up)
                                        {
                                                keymap.V_Most_Recent_Press_Up = true;
                                                keymap.move_up_pressed = true;
                                                return true;
                                        }
                                        if(e->key == keymap.move_down)
                                        {
                                                keymap.V_Most_Recent_Press_Up = false;
                                                keymap.move_down_pressed = true;
                                                return true;
                                        }
                                        if(e->key == keymap.move_left)
                                        {
                                                keymap.H_Most_Recent_Press_Left = true;
                                                keymap.move_left_pressed = true;
                                                return true;
                                        }
                                        if(e->key == keymap.move_right)
                                        {
                                                keymap.H_Most_Recent_Press_Left = false;
                                                keymap.move_right_pressed = true;
                                                return true;
                                        }
                                }
                                if (e->action == GLFW_RELEASE)
                                {
                                        if(e->key == keymap.move_up)
                                        {
                                                keymap.V_Most_Recent_Press_Up = false;
                                                keymap.move_up_pressed = false;
                                                return true;
                                        }
                                        if(e->key == keymap.move_down)
                                        {
                                                keymap.V_Most_Recent_Press_Up = true;
                                                keymap.move_down_pressed = false;
                                                return true;
                                        }
                                        if(e->key == keymap.move_left)
                                        {
                                                keymap.H_Most_Recent_Press_Left = false;
                                                keymap.move_left_pressed = false;
                                                return true;
                                        }
                                        if(e->key == keymap.move_right)
                                        {
                                                keymap.H_Most_Recent_Press_Left = true;
                                                keymap.move_right_pressed = false;
                                                return true;
                                        }
                                }
                                return false;
                        });
                        glm::vec2 dir = keymap.direction();
                        vel.velocity.x = dir.x * 450;
                        vel.velocity.y = dir.y * 450;

                        //set the integer direction of the player
                        auto anim_dir = keymap.animation_direction();
                        if (anim_dir != -1)
                                direction.movement_direction = anim_dir;
                }
        });
}
