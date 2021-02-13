#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "components/PlayerKeymap.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

#include "rendering/Renderer.hpp"
#include "core/game.hpp"


Keyboard_Input_Direction::Keyboard_Input_Direction()
{
        move_up = glfwGetKeyScancode(GLFW_KEY_W);
        move_down = glfwGetKeyScancode(GLFW_KEY_S);
        move_left = glfwGetKeyScancode(GLFW_KEY_A);
        move_right = glfwGetKeyScancode(GLFW_KEY_D);
}

void Keyboard_Input_Direction::update(float character_x, float character_y)
{
        Events::iterate<E_Keyboard>([this](auto &e) {
                if (e->action == GLFW_PRESS)
                {
                        if (e->scancode == move_up)
                        {
                                V_Most_Recent_Press_Up = true;
                                move_up_pressed = true;
                                return true;
                        }
                        if (e->scancode == move_down)
                        {
                                V_Most_Recent_Press_Up = false;
                                move_down_pressed = true;
                                return true;
                        }
                        if (e->scancode == move_left)
                        {
                                H_Most_Recent_Press_Right = false;
                                move_left_pressed = true;
                                return true;
                        }
                        if (e->scancode == move_right)
                        {
                                H_Most_Recent_Press_Right = true;
                                move_right_pressed = true;
                                return true;
                        }
                }
                if (e->action == GLFW_RELEASE)
                {
                        if (e->scancode == move_up)
                        {
                                V_Most_Recent_Press_Up = false;
                                move_up_pressed = false;
                                return true;
                        }
                        if (e->scancode == move_down)
                        {
                                V_Most_Recent_Press_Up = true;
                                move_down_pressed = false;
                                return true;
                        }
                        if (e->scancode == move_left)
                        {
                                H_Most_Recent_Press_Right = true;
                                move_left_pressed = false;
                                return true;
                        }
                        if (e->scancode == move_right)
                        {
                                H_Most_Recent_Press_Right = false;
                                move_right_pressed = false;
                                return true;
                        }
                }
                return false;
        });
}

std::bitset<4> Keyboard_Input_Direction::direction_buttons()
{
        std::bitset<4> out;
        out[0] = move_left_pressed || move_right_pressed;
        out[1] = H_Most_Recent_Press_Right;
        out[2] = move_up_pressed || move_down_pressed;
        out[3] = V_Most_Recent_Press_Up;
        return out;
}

int8_t Keyboard_Input_Direction::direction()
{
        constexpr int8_t directions[3][3] = {
                {5, 6, 7},
                {4, -1, 0},
                {3, 2, 1}};

        std::bitset<4> buttons = direction_buttons();
        int x = buttons[0] ? (buttons[1] ? 2 : 0) : 1;
        int y = buttons[2] ? (buttons[3] ? 2 : 0) : 1;
        return(directions[x][y]);
}


void Mouse_Input_Direction::update(float character_x, float character_y)
{
        glm::vec2 mouse_world_pos = Renderer::screenToWorld(glm::vec2(Game::cursor_x, Game::cursor_y));
        glm::vec2 mouse_dir = glm::vec2(character_x, character_y) - mouse_world_pos;
        mouse_direction = int(glm::round(((glm::atan(-mouse_dir.x, mouse_dir.y) / glm::pi<float>()) + 1) * 4) + 4) % 8;
}

int8_t Mouse_Input_Direction::direction()
{
        return mouse_direction;
}

std::bitset<4> Mouse_Input_Direction::direction_buttons()
{
        constexpr std::bitset<4> button_combos[9] = {
                0b0000,
                0b1100,
                0b1111,
                0b0011,
                0b0111,
                0b0100,
                0b0101,
                0b0001,
                0b1101
        };
        return button_combos[mouse_direction + 1];
}