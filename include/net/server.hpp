#pragma once
#include "net/common.hpp"
#include "net/message.hpp"

namespace net
{
template <typename T>
class server_interface
{
public:
        server_interface()
        {
        }
        ~server_interface()
        {
        }

        void Update()
        {
                ENetEvent event;
                while (enet_host_service(server, &event, 0) > 0)
                {
                        switch (event.type)
                        {
                        case ENET_EVENT_TYPE_CONNECT:
                                std::cout << "Client connected" << std::endl;
                                break;

                        case ENET_EVENT_TYPE_RECEIVE:
                        {
                                enet_packet_destroy(event.packet);
                                message<T> msg;
                                memcpy(&msg.header, event.packet->data, sizeof(message_header<T>));
                                msg.body.resize(msg.header.body_size);
                                std::cout << "size of body: " << msg.header.body_size << std::endl;
                                memcpy(msg.body.data(), event.packet->data + sizeof(message_header<T>), msg.body.size());
                                OnMessage(msg);
                        }
                        break;

                        case ENET_EVENT_TYPE_DISCONNECT:
                                event.peer->data = NULL;
                                break;
                        }
                }
        }

        bool Start()
        {
                address.host = ENET_HOST_ANY;
                address.port = 1234;
                server = enet_host_create(&address, 32, 2, 0, 0);
                if (server == NULL)
                {
                        std::cout << "An error occurred while trying to make a server" << std::endl;
                }
                else
                {
                        std::cout << "Server created" << std::endl;
                }
                return true;
        }

        void Stop()
        {
        }
        void MessageClient(const message<T>& msg, ENetPeer* peer)
        {
                ENetPacket *packet = enet_packet_create(&msg.header, sizeof(message_header<T>), ENET_PACKET_FLAG_RELIABLE);
                if (msg.header.body_size > 0)
                {
                        enet_packet_resize(packet, msg.size());
                        memcpy(&packet->data[sizeof(message_header<T>)], msg.body.data(), msg.header.body_size);
                }
                enet_peer_send(peer, 0, packet);
        }
        void flush()
        {
                enet_host_flush(server);
        }

private:
        virtual bool OnClientConnect(ENetPeer* peer)
        {
                return false;
        }
        virtual void OnClientDisconnect(ENetPeer* peer) = 0;
        virtual void OnMessage(message<T>& msg) = 0;
        ENetAddress address;
        ENetHost* server = nullptr;
};
}
