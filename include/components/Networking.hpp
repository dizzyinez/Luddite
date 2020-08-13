#ifndef C_Networking_hpp
#define C_Networking_hpp

#include <enet/enet.h>
#include "entt.hpp"
#include "events/logging.hpp"
#include <bitset>

struct C_Net_Client
{
        C_Net_Client()
        {
        }

        ENetHost* client;
        ENetPeer* server;
        ENetEvent event;
        bool initialized = false;
        void setInitialized(bool init)
        {
                initialized = init;
        }
};

struct C_Net_Host
{
        C_Net_Host() = default;

        ENetHost* server;
        ENetEvent event;
        bool initialized = false;
};

#endif
