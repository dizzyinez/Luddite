#include "systems/Networking.hpp"
#include <enet/enet.h>
#include "events/Events.hpp"
#include "events/Logging.hpp"
#include "events/Networking.hpp"
#include "core/PacketDefinitions.hpp"
#include "components/Networking.hpp"

#include <iostream>

#include <bitset>
#include <climits>

template<typename T>
void show_binrep(const T& a)
{
        const char* beg = reinterpret_cast<const char*>(&a);
        const char* end = beg + sizeof(a);
        while(beg != end)
                std::cout << std::bitset<CHAR_BIT>(*beg++) << ' ';
        std::cout << '\n';
}




void S_Net_Client::update(float deltaTime, entt::registry &reg)
{
        auto &client = reg.ctx<C_Net_Client>();
        //handle connection
        for (auto be : *Events::get<E_Net_Connect>())
        {
                auto e = std::static_pointer_cast<E_Net_Connect>(be);
                client.client = enet_host_create(NULL, 1, 2, 0, 0);
                if (client.client == NULL)
                        LOG_DEBUG("Client failed to initialize!");
                else{
                        LOG_DEBUG("Client initialized");
                }


                ENetAddress address;
                enet_address_set_host(&address, "127.0.0.1");
                // enet_address_set_host(&address, e->hostName);
                address.port = 1234;                                //e->port;

                client.server = enet_host_connect(client.client, &address, 2, 0);
                if(client.server == NULL)
                {
                        LOG_DEBUG("No available servers for initiating an ENet connection!");
                }
                std::cout << client.server->channelCount << std::endl;
                if (enet_host_service(client.client, &client.event, 1000) > 0 &&
                    client.event.type == ENET_EVENT_TYPE_CONNECT)
                {
                        P_Connect_Request rq;
                        ENetPacket* packet = convertToPacket(rq);
                        enet_peer_send(client.server, 0, packet);
                        LOG_DEBUG("Connected to host");
                        client.initialized = true;
                }
                else
                {
                        // enet_peer_reset(client.server);
                        // client.server = nullptr;
                        // log("Failed to connect to host");
                }
                e->setHandled();
                //return true;                 //event was handled
        }
        //handle disconnection
        Events::iterate<E_Net_Disconnect>([client](auto &e) {
                enet_peer_disconnect(client.server, 0);
                return true;
        });

        if (client.initialized == true)
        {
                // P_Fatal_Error event("ahahahhahahaha");
                // ENetPacket* packet = convertToPacket(event);
                // Events::emit<E_Net_Send>(packet);
                while (enet_host_service(client.client, &client.event, 0) > 0)
                {
                        switch (client.event.type)
                        {
                        case ENET_EVENT_TYPE_CONNECT:
                                LOG_DEBUG("A new client connected!");
                                break;
                        case ENET_EVENT_TYPE_RECEIVE:
                                switch (client.event.packet->data[0]) {
                                case 1:
                                {
                                        P_Fatal_Error p = convertFromPacket<P_Fatal_Error>(client.event);
                                        // LOG_DEBUG(p.error_message);
                                        // std::cout << "stored: " << strlen(p.error_message) << " copied: " << host.event.packet->dataLength << " message: " << p.error_message << std::endl;
                                        break;
                                }
                                case 3:
                                {
                                        P_Connect_Approved p = convertFromPacket<P_Connect_Approved>(client.event);
                                        p.on_client_receive(reg);
                                        break;
                                }
                                }
                                enet_packet_destroy(client.event.packet);
                                break;
                        case ENET_EVENT_TYPE_DISCONNECT:
                                LOG_DEBUG("disconected from host");
                                break;

                        }
                }
        }
}
