#ifndef E_Networking
#define E_Networking

#include "events/Events.hpp"
#include <enet/enet.h>
#include <string>


struct E_Net_Connect : public Event<E_Net_Connect>
{
        E_Net_Connect(const char * hostName_, uint16_t port_)
                : hostName(hostName_), port(port_)
        {
        }
        const char * hostName;
        uint16_t port;
};


struct E_Net_Disconnect : public Event<E_Net_Disconnect>
{
        E_Net_Disconnect()
        {
        }
        //std::string reason; ????
};

struct E_Net_Host : public Event<E_Net_Host>
{
        E_Net_Host(uint16_t port_, uint8_t max_players_)
                : port(port_), max_players(max_players_)
        {
        }
        uint16_t port;
        uint8_t max_players;
};

struct E_Net_Send : public Event<E_Net_Send>
{
        E_Net_Send(ENetPacket* packet_)
                : packet(packet_)
        {

        }
        // ~E_Net_Send() //free packet
        ENetPacket* packet;
};
#endif
