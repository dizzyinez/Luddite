#include "systems/Networking.hpp"
#include <enet/enet.h>
#include "events/Logging.hpp"
#include "events/Networking.hpp"
#include "core/PacketDefinitions.hpp"
#include "components/Networking.hpp"

#include <iostream>

#include <string>

std::string string_to_hex(const std::string& input)
{
        static const char hex_digits[] = "0123456789ABCDEF";

        std::string output;
        output.reserve(input.length() * 2);
        for (unsigned char c : input)
        {
                output.push_back(hex_digits[c >> 4]);
                output.push_back(hex_digits[c & 15]);
        }
        return output;
}



void S_Net_Host::update(float deltaTime, entt::registry &reg)
{
        auto &host = reg.ctx<C_Net_Host>();
        for (auto it = (*Events::get<E_Net_Host>()).begin(); it != (*Events::get<E_Net_Host>()).end(); it++)
        // for (auto host_event : *Events::get<E_Net_Host>())
        {
                auto e = std::static_pointer_cast<E_Net_Host>(*it);
                ENetAddress address;
                address.host = ENET_HOST_ANY;
                address.port = e->port;
                // std::cout << host.server->address.port << std::endl;
                host.server = enet_host_create (&address, e->max_players - 1, 2, 0, 0);
                if (host.server == NULL)
                {
                        LOG_DEBUG("Server failed to initialize!");
                }
                else
                {
                        LOG_DEBUG("Server initialized");
                        host.initialized = true;
                }
                e->setHandled();
                // (*it)->setHandled();
                // std::cout << "event handled: " <<  (*it)->isHandled() << std::endl;
                // return true;
        }
        // std::cout << "times: " << times << std::endl;

        // std::cout << host.server->address.port << std::endl;
        if (host.initialized == true)
        {
                // P_Connect_Approved p;
                // p.x = 0.0f;
                // p.y = 0.0f;
                // p.player_slot = 50;
                // // P_Player_Control c = convertFromPacket<P_Player_Control>(convertToPacket(p));
                // P_Connect_Approved c = convertFromPacket<P_Connect_Approved>(convertToPacket(p));
                // std::cout << "slot: " << static_cast<int>(c.player_slot) << "    should be " << static_cast<int>(p.player_slot) << std::endl;



                while (enet_host_service(host.server, &host.event, 0) > 0)
                {
                        switch (host.event.type)
                        {
                        case ENET_EVENT_TYPE_CONNECT:
                                // LOG_DEBUG("A new client connected!");
                                std::cout << "A new client connected!" << std::endl;
                                break;
                        case ENET_EVENT_TYPE_RECEIVE:
                                // std::cout << "recieved a packet from a client" << std::endl;
                                // LOG_DEBUG("recieved a packet from a client!");
                                // std::cout << "data" << host.event.packet->data << std::endl;
                                switch (host.event.packet->data[0])
                                {
                                case 1:
                                {
                                        P_Fatal_Error p = convertFromPacket<P_Fatal_Error>(host.event.packet);
                                        // LOG_DEBUG(p.error_message);
                                        // std::cout << "stored: " << strlen(p.error_message) << " copied: " << host.event.packet->dataLength << " message: " << p.error_message << std::endl;
                                        break;
                                }
                                case 2:
                                {
                                        std::cout << "new connection request" << std::endl;
                                        P_Connect_Request p = convertFromPacket<P_Connect_Request>(host.event.packet);
                                        std::cout << "packet converted" << std::endl;
                                        p.on_host_receive(host.event.peer, reg);
                                        break;
                                }
                                case 6:
                                {
                                        P_Player_Control p = convertFromPacket<P_Player_Control>(host.event.packet);
                                        p.on_host_receive(host.event.peer, reg);
                                        break;
                                }
                                }
                                enet_packet_destroy(host.event.packet);
                                break;
                        case ENET_EVENT_TYPE_DISCONNECT:
                                std::cout << (host.event.type == ENET_EVENT_TYPE_DISCONNECT) << std::endl;
                                // LOG_DEBUG("a client disconnected!");
                                std::cout << "a client disconnected!" << std::endl;
                                break;
                        }
                }
        }
}
