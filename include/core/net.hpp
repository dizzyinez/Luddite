#pragma once
#include "net/message.hpp"
#include "net/server.hpp"
#include "net/client.hpp"

enum class message_types : uint32_t
{
        CONNECTION_REQUEST,
        DISCONNECTION
};

using Message = net::message<message_types>;

class Server : public net::server_interface<message_types>
{
        bool OnClientConnect(ENetPeer* peer);
        void OnClientDisconnect(ENetPeer* peer);
        void OnMessage(Message& msg);
};

class Client : public net::client_interface<message_types>
{
};