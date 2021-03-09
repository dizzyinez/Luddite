#pragma once
#include "entt.hpp"
#include "events/Logging.hpp"
#include "core/net.hpp"
// #include <bitset>

struct C_Net_Client
{
        C_Net_Client()
        {
        }
        bool initialized = false;
        void setInitialized(bool init)
        {
                initialized = init;
        }
        Client client;
};

struct C_Net_Server
{
        C_Net_Server()
        {
        }
        bool initialized = false;
        Server server;
};
