#include "systems/LocalPlayerInput.hpp"
#include "components/player.hpp"
#include "components/playerkeymap.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"


void S_LocalPlayerInput::update(float deltaTime, entt::registry &reg)
{
        auto view = reg.view<C_PlayerKeymap>();
        for (auto entity : view)
        {
                auto &keymap = reg.get<C_PlayerKeymap>(entity);
                Events::iterate<E_Keyboard>([&keymap](auto &e) {
                        if (e->action == GLFW_PRESS)
                        {
                                if (e->key == keymap.move_up)
                                {
                                        keymap.V_Most_Recent_Press_Up = true;
                                        keymap.move_up_pressed = true;
                                        return true;
                                }
                                if (e->key == keymap.move_down)
                                {
                                        keymap.V_Most_Recent_Press_Up = false;
                                        keymap.move_down_pressed = true;
                                        return true;
                                }
                                if (e->key == keymap.move_left)
                                {
                                        keymap.H_Most_Recent_Press_Right = false;
                                        keymap.move_left_pressed = true;
                                        return true;
                                }
                                if (e->key == keymap.move_right)
                                {
                                        keymap.H_Most_Recent_Press_Right = true;
                                        keymap.move_right_pressed = true;
                                        return true;
                                }
                        }
                        if (e->action == GLFW_RELEASE)
                        {
                                if (e->key == keymap.move_up)
                                {
                                        keymap.V_Most_Recent_Press_Up = false;
                                        keymap.move_up_pressed = false;
                                        return true;
                                }
                                if (e->key == keymap.move_down)
                                {
                                        keymap.V_Most_Recent_Press_Up = true;
                                        keymap.move_down_pressed = false;
                                        return true;
                                }
                                if (e->key == keymap.move_left)
                                {
                                        keymap.H_Most_Recent_Press_Right = true;
                                        keymap.move_left_pressed = false;
                                        return true;
                                }
                                if (e->key == keymap.move_right)
                                {
                                        keymap.H_Most_Recent_Press_Right = false;
                                        keymap.move_right_pressed = false;
                                        return true;
                                }
                        }
                        return false;
                });

                C_PlayerInput &input = reg.get<C_PlayerInput>(entity);
                input.buttons[0] = (keymap.move_left_pressed || keymap.move_right_pressed);
                input.buttons[1] = (keymap.H_Most_Recent_Press_Right);
                input.buttons[2] = (keymap.move_up_pressed || keymap.move_down_pressed);
                input.buttons[3] = (keymap.V_Most_Recent_Press_Up);
        }
}
