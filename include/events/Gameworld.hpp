#ifndef E_Gameworld_hpp
#define E_Gameworld_hpp

#include "events/Events.hpp"

struct E_SpawnPlayer : public Event<E_SpawnPlayer>
{
        E_SpawnPlayer(bool localPlayer_)
                : localPlayer(localPlayer_)
        {
        }
        E_SpawnPlayer(uint8_t slot_)
                : slot(slot_)
        {
        }
        E_SpawnPlayer(uint8_t slot_, float xpos_, float ypos_)
                : slot(slot_), xpos(xpos_), ypos(ypos_)
        {
        }
        bool localPlayer = false;
        uint8_t slot = 0;
        float xpos = 0.0f;
        float ypos = 0.0f;
};

#endif
