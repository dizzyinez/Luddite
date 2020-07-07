#ifndef Player_hpp
#define Player_hpp
#include "entityx/entityx.h"


struct Player : entityx::Component<Player>
{
        bool localPlayer = true;
};

#endif
