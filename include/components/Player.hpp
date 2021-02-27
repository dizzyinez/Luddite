#ifndef C_Player_hpp
#define C_Player_hpp
#include <iostream>
#include "ecs/Entity.hpp"
#include <bitset>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include <glm/gtx/string_cast.hpp>
constexpr int TOTAL_DIRECTIONS = 32;

struct C_Player
{
        C_Player(bool local_player_, uint8_t player_slot_)
                : local_player(local_player_), player_slot(player_slot_)
        {
        }
        bool local_player = false;
        uint8_t player_slot = 0;
        float trauma = 0.f;
};

struct C_PlayerSlots
{
        C_PlayerSlots()
        {
                for (int i = 0; i < 256; i++)
                {
                        players[i] = entt::null;
                }
        }
        entt::entity players[256];
};

struct C_PlayerDirection
{
        C_PlayerDirection() = default;
        int8_t movement_direction = 0;
};

struct C_PlayerInput
{
        std::bitset<8> buttons{0x00};
        int8_t mouse_direction = 2;
        bool net_validated = false;
        /*
         * 0 Is moving horizontally
         * 1 Is moving right
         * 2 Is moving vertically
         * 3 Is moving up
         * 4 Button 1
         * 5 Button 2
         * 6 Button 3
         * 7 Button 4
         */

        int moveX()
        {
                return (buttons[0] == 1) ? ((buttons[1] == 1) ? 1 : -1) : 0;
        }
        int moveY()
        {
                return (buttons[2] == 1) ? ((buttons[3] == 1) ? -1 : 1) : 0;
        }
        glm::vec2 direction()
        {
                glm::vec2 dir = glm::vec2(
                        (buttons[0] == 1) ? ((buttons[1] == 1) ? 1.0 : -1.0) : 0.0,
                        (buttons[2] == 1) ? ((buttons[3] == 1) ? -1.0 : 1.0) : 0.0
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
        int8_t animation_direction()
        {
                constexpr int8_t directions[3][3] = {
                        {5, 6, 7},
                        {4, -1, 0},
                        {3, 2, 1}};
                int x = buttons[0] ? (buttons[1] ? 2 : 0) : 1;
                int y = buttons[2] ? (buttons[3] ? 2 : 0) : 1;
                return(directions[x][y] * (TOTAL_DIRECTIONS / 8));
        }

        bool button1()
        {
                return buttons[4];
        }
        bool button2()
        {
                return buttons[5];
        }
        bool button3()
        {
                return buttons[6];
        }
        bool button4()
        {
                return buttons[7];
        }
};

#endif
