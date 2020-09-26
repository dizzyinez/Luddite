#ifndef Player_Keymap_hpp
#define Player_Keymap_hpp

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

        bool H_Most_Recent_Press_Right;
        bool V_Most_Recent_Press_Up;
};
#endif
