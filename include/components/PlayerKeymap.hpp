#ifndef Player_Keymap_hpp
#define Player_Keymap_hpp

#include <GLFW/glfw3.h>
#include <bitset>
#include <memory>
#include <array>

enum class Input_Devices : uint8_t
{
        KEYBOARD,
        MOUSE,
        GAMEPAD
};

struct Input_Binding
{
        Input_Devices device;
        int code;
        bool held = false;
        // int mods;
};

struct Input_Direction
{
        // Input_Devices device;
        // int codes[4];
        // static Input_Direction keyboard_direction(N_code, S_code, E_code, W_code) {return{Input_Devices::KEYBOARD N_code, S_code, E_code, W_code};}
        // static Input_Direction mouse_direction() {return{Input_Devices::MOUSE 0, 0, 0, 0};}

        virtual void update(float character_x, float character_y) = 0;
        virtual std::bitset<4> direction_buttons() = 0;
        virtual int8_t direction() = 0;
};

struct Keyboard_Input_Direction : Input_Direction
{
        Keyboard_Input_Direction();
        int move_up;
        int move_down;
        int move_left;
        int move_right;

        bool move_up_pressed = false;
        bool move_down_pressed = false;
        bool move_left_pressed = false;
        bool move_right_pressed = false;

        bool H_Most_Recent_Press_Right;
        bool V_Most_Recent_Press_Up;

        void update(float character_x, float character_y);
        std::bitset<4> direction_buttons();
        int8_t direction();
};

struct Mouse_Input_Direction : Input_Direction
{
        int8_t mouse_direction;

        void update(float character_x, float character_y);
        std::bitset<4> direction_buttons();
        int8_t direction();
};

struct C_PlayerKeymap
{
        C_PlayerKeymap()
        {
                // motion = std::make_unique<Keyboard_Input_Direction>();
                motion = std::unique_ptr<Keyboard_Input_Direction>(new Keyboard_Input_Direction());
                look = std::unique_ptr<Mouse_Input_Direction>(new Mouse_Input_Direction());
                binds[0] = {Input_Devices::MOUSE, GLFW_MOUSE_BUTTON_LEFT};
                binds[1] = {Input_Devices::MOUSE, 1};
                binds[2] = {Input_Devices::KEYBOARD, glfwGetKeyScancode(GLFW_KEY_LEFT_SHIFT)};
                binds[3] = {Input_Devices::KEYBOARD, glfwGetKeyScancode(GLFW_KEY_SPACE)};
        }
        std::unique_ptr<Input_Direction> motion;
        std::unique_ptr<Input_Direction> look;
        std::array<Input_Binding, 4> binds;
};
#endif
