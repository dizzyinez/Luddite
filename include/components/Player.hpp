#ifndef C_Player_hpp
#define C_Player_hpp
#include <iostream>

struct C_Player
{
        C_Player() = default;
        C_Player(bool localPlayer_, uint8_t playerSlot_)
                : localPlayer(localPlayer_), playerSlot(playerSlot_)
        {
        }
        bool localPlayer = false;
        uint8_t playerSlot = 0;
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
