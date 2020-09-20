#ifndef C_Player_hpp
#define C_Player_hpp
#include <iostream>
#include "ecs/Entity.hpp"

struct C_Player
{
        C_Player() = default;
        C_Player(bool local_player_, uint8_t player_slot_)
                : local_player(local_player_), player_slot(player_slot_)
        {
        }
        bool local_player = false;
        uint8_t player_slot = 0;
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
        int8_t mouse_direction = 0;
};

#endif
