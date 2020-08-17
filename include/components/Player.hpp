#ifndef C_Player_hpp
#define C_Player_hpp
#include <iostream>

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

#endif
