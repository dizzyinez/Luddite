#include "systems/LocalPlayerInput.hpp"
#include "components/player.hpp"
#include "components/playerkeymap.hpp"
#include "components/Position.hpp"
#include "components/Origin.hpp"
#include "components/Size.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"
#include "rendering/Renderer.hpp"
#include "core/game.hpp"

void S_LocalPlayerInput::update(float deltaTime, entt::registry &reg)
{
        auto view = reg.view<C_PlayerKeymap>();
        for (auto entity : view)
        {
                auto &&[keymap, position, origin, input] = reg.get<C_PlayerKeymap, C_Position, C_Origin, C_PlayerInput>(entity);
                glm::vec2 char_pos = glm::vec2(position.position) + origin.origin;
                keymap.motion->update(char_pos.x, char_pos.y);
                keymap.look->update(char_pos.x, char_pos.y);


                Events::iterate<E_Keyboard>([&keymap, &input](auto &e) {
                        bool handled;
                        for (int i = 0; i < keymap.binds.size(); i++)
                        {
                                Input_Binding& bind = keymap.binds.at(i);
                                if (bind.device == Input_Devices::KEYBOARD)
                                {
                                        if (bind.code == e->scancode)
                                        {
                                                if (e->action == GLFW_PRESS)
                                                {
                                                        bind.held = true;
                                                        // input.buttons[4 + i] = 1;
                                                        handled = true;
                                                }
                                                if (e->action == GLFW_RELEASE)
                                                {
                                                        bind.held = false;
                                                        // input.buttons[4 + i] = 0;
                                                        handled = true;
                                                }
                                        }
                                }
                        }
                        return handled;
                });

                Events::iterate<E_MouseButton>([&keymap, &input](auto& e) {
                        bool handled;
                        for (int i = 0; i < keymap.binds.size(); i++)
                        {
                                Input_Binding& bind = keymap.binds.at(i);
                                if (bind.device == Input_Devices::MOUSE)
                                {
                                        if (bind.code == e->button)
                                        {
                                                if (e->action == GLFW_PRESS)
                                                {
                                                        bind.held = true;
                                                        // input.buttons[4 + i] = 1;
                                                        handled = true;
                                                }
                                                if (e->action == GLFW_RELEASE)
                                                {
                                                        bind.held = false;
                                                        // input.buttons[4 + i] = 0;
                                                        handled = true;
                                                }
                                        }
                                }
                        }
                        return handled;
                });

                std::bitset<4> dir_buttons = keymap.motion->direction_buttons();
                input.buttons[0] = dir_buttons[0];
                input.buttons[1] = dir_buttons[1];
                input.buttons[2] = dir_buttons[2];
                input.buttons[3] = dir_buttons[3];
                input.buttons[4] = keymap.binds.at(0).held;
                input.buttons[5] = keymap.binds.at(1).held;
                input.buttons[6] = keymap.binds.at(2).held;
                input.buttons[7] = keymap.binds.at(3).held;

                // input.animation_direction = keymap.motion->direction();
                input.mouse_direction = keymap.look->direction();
        }
}
