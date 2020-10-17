#include "core/net.hpp"


bool Server::OnClientConnect(ENetPeer* peer)
{
        return true;
}

void Server::OnClientDisconnect(ENetPeer* peer)
{
}

void Server::OnMessage(Message& msg)
{
        switch (msg.header.id)
        {
        case message_types::CONNECTION_REQUEST:
                int version_major;
                int version_minor;
                msg >> version_minor >> version_major;
                std::cout << "connection requested, version: " << version_major << "." << version_minor << std::endl;
                break;

        case message_types::DISCONNECTION:
                std::cout << "client disconnected" << std::endl;
                break;
        }
}