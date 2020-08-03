#ifndef C_Networking_hpp
#define C_Networking_hpp

#include <enet/enet.h>
#include "events/logging.hpp"

struct C_Net_Client
{
        C_Net_Client(size_t peerCount, size_t channelLimit, enet_uint32 incomingBandwidth, enet_uint32 outgoingBandwidth)
                : client(enet_host_create(NULL, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth))
        {
                if (client == NULL)
                        Events::emit<E_Log>("Client failed to initialize!");
                else{
                        Events::emit<E_Log>("Client initialized");
                }
        }

        std::unique_ptr<ENetHost> client;
        ENetAddress address;
        ENetEvent event;
        std::unique_ptr<ENetPeer> peer;
};

#endif
