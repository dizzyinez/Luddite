#include "systems/Networking.hpp"
#include <enet/enet.h>
#include "events/Logging.hpp"
#include "events/Networking.hpp"
#include "core/PacketDefinitions.hpp"
#include "components/Networking.hpp"

#include <iostream>

void S_Net_Host::update(float deltaTime, entt::registry &reg)
{
        auto &host = reg.ctx<C_Net_Host>();
        int times = 0;
        for (auto it = (*Events::get<E_Net_Host>()).begin(); it != (*Events::get<E_Net_Host>()).end(); it++)
        // for (auto host_event : *Events::get<E_Net_Host>())
        {
                auto e = std::static_pointer_cast<E_Net_Host>(*it);
                ENetAddress address;
                address.host = ENET_HOST_ANY;
                address.port = 1234;                //e->port;
                // std::cout << host.server->address.port << std::endl;
                host.server = enet_host_create (&address, 32 /*e->max_players /*- 1*/, 2, 0, 0);
                if (host.server == NULL)
                {
                        LOG_DEBUG("Server failed to initialize!");
                }
                else
                {
                        // LOG_DEBUG("Server initialized");
                        host.initialized = true;
                }
                e->setHandled();
                // (*it)->setHandled();
                // std::cout << "event handled: " <<  (*it)->isHandled() << std::endl;
                // return true;
                times++;
        }
        // std::cout << "times: " << times << std::endl;

        // std::cout << host.server->address.port << std::endl;
        if (host.initialized == true)
        {
                // std::cout << "test" << std::endl;
                while (enet_host_service(host.server, &host.event, 0) > 0)
                {
                        switch (host.event.type)
                        {
                        case ENET_EVENT_TYPE_CONNECT:
                                LOG_DEBUG("A new client connected!");
                                break;
                        case ENET_EVENT_TYPE_RECEIVE:
                                // std::cout << "recieved a packet from a client" << std::endl;
                                // LOG_DEBUG("recieved a packet from a client!");
                                // std::cout << "data" << host.event.packet->data << std::endl;
                                switch (host.event.packet->data[0]) {
                                case 1:
                                {
                                        P_Fatal_Error p = convertFromPacket<P_Fatal_Error>(host.event);
                                        // LOG_DEBUG(p.error_message);
                                        // std::cout << "stored: " << strlen(p.error_message) << " copied: " << host.event.packet->dataLength << " message: " << p.error_message << std::endl;
                                        break;
                                }
                                case 2:
                                {
                                        P_Connect_Request p = convertFromPacket<P_Connect_Request>(host.event);
                                        p.on_host_receive(host.event.peer, reg);
                                        break;
                                }
                                }
                                enet_packet_destroy(host.event.packet);
                                break;
                        case ENET_EVENT_TYPE_DISCONNECT:
                                LOG_DEBUG("a client disconnected!");
                                break;
                        }
                }
        }
}
