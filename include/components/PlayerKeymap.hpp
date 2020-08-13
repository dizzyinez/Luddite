#ifndef Player_Keymap_hpp
#define Player_Keymap_hpp

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

struct C_PlayerKeymap
{
        C_PlayerKeymap() = default;
        uint16_t move_up = GLFW_KEY_W;
        uint16_t move_down = GLFW_KEY_S;
        uint16_t move_left = GLFW_KEY_A;
        uint16_t move_right = GLFW_KEY_D;

        bool move_up_pressed = false;
        bool move_down_pressed = false;
        bool move_left_pressed = false;
        bool move_right_pressed = false;

        bool H_Most_Recent_Press_Left;
        bool V_Most_Recent_Press_Up;
        glm::vec2 direction()
        {
                glm::vec2 dir = glm::vec2(
                        (move_left_pressed || move_right_pressed) ? (H_Most_Recent_Press_Left ? -1.0f : 1.0f) : 0.0f,
                        (move_up_pressed   || move_down_pressed ) ? (V_Most_Recent_Press_Up   ? -1.0f : 1.0f) : 0.0f
                        );
                if (dir != glm::vec2(0.0f, 0.0f))
                {
                        return glm::normalize(dir);
                }
                else
                {
                        return dir;
                }
        }
};
#endif
