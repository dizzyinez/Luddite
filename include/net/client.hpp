#pragma once
#include "net/common.hpp"
#include "net/message.hpp"

#include <enet/enet.h>
namespace net
{
template <typename T>
class client_interface
{
public:
        client_interface()
        {
                client = enet_host_create(NULL, 1, 2, 0, 0);
                if (client == NULL)
                {
                        std::cout << "An error occurred while trying to make a client" << std::endl;
                }
                else
                {
                        std::cout << "Client created successfully" << std::endl;
                }
        }
        ~client_interface() {}
        bool Connect()
        {
                enet_address_set_host_ip(&server_address, "127.0.0.1");
                server_address.port = 1234;
                server = enet_host_connect(client, &server_address, 2, 0);
                if (server == NULL)
                {
                        std::cout << "No available peer" << std::endl;
                }
                ENetEvent event;
                if (enet_host_service(client, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
                {
                        std::cout << "connection successful" << std::endl;
                        return true;
                }
                std::cout << "connection unsuccessful" << std::endl;
                return false;
        }
        void Disconnect() {}
        bool IsConnected() {return true;}
        void Update() {}
        void MessageServer(const message<T>& msg)
        {
                ENetPacket *packet = enet_packet_create(&msg.header, sizeof(message_header<T>), ENET_PACKET_FLAG_RELIABLE);
                if (msg.header.body_size > 0)
                {
                        enet_packet_resize(packet, msg.size());
                        memcpy(&packet->data[sizeof(message_header<T>)], msg.body.data(), msg.header.body_size);
                }
                enet_peer_send(server, 0, packet);
        }
        void flush()
        {
                enet_host_flush(client);
        }

private:
        ENetHost* client;
        ENetAddress server_address;
        ENetPeer* server;
};
}