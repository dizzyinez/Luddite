#ifndef PacketDefinitions_hpp
#define PacketDefinitions_hpp

#include "entt.hpp"
#include <enet/enet.h>
#include <unordered_map>
#include <ios>

template <typename T>
ENetPacket* convertToPacket(T p)
{
        char data[sizeof(p)];
        memcpy(data, &p, sizeof(p));
        return enet_packet_create(data, strlen(data), ENET_PACKET_FLAG_RELIABLE);
};

template <typename T>
T convertFromPacket(ENetEvent &event)
{
        T data;
        //this probably causes a memory leak when
        //the data doesn't have a char array in it.
        //I couldn't find another way to do this
        //tho, so this is what we get.
        strcpy(static_cast<char*>(static_cast<void*>(&data)), static_cast<char*>(static_cast<void*>(event.packet->data)));
        return data;
}

struct P_Fatal_Error
{
        P_Fatal_Error() = default;
        P_Fatal_Error(char* error_message_)
        {
                strcpy(error_message, error_message_);
        }


        uint8_t id = 1;
        char error_message[200];
};

struct P_Connect_Request
{
        P_Connect_Request() = default;
        uint8_t id = 2;
        char version[11] = "Luddite001";
        void on_host_receive(ENetPeer* peer, entt::registry &reg);
};

struct P_Connect_Approved
{
        P_Connect_Approved() = default;
        P_Connect_Approved(uint8_t Playerid_)
                : Playerid(Playerid_)
        {
        }
        uint8_t id = 3;
        uint8_t Playerid;
        void on_client_receive(entt::registry &reg);
};

struct P_Connect_Denied
{
        P_Connect_Denied() = default;
        uint8_t id = 4;
        char version[20] = "Luddite001";
        // void on_client_receive(entt::registry &reg);
};

struct P_Spawn_Player
{
        P_Spawn_Player() = default;
        P_Spawn_Player(uint8_t slot_)
                : slot(slot_)
        {
        }
        P_Spawn_Player(uint8_t slot_, float x_, float y_)
                : slot(slot_), x(x_), y(y_)
        {
        }
        uint8_t id = 4;
        uint8_t slot = 4;
        float x = 0.0f;
        float y = 0.0f;
        void on_client_receive(entt::registry &reg);
};

struct P_Player_Control
{
        P_Player_Control()
        {
        }
        // P_Player_Control(char* error_message_)
        // {
        //         memcpy(error_message, error_message_, sizeof(error_message));
        // }
        uint8_t id = 5;
        float xpos = 420.69f;
        float ypos = 420.69f;
        // char error_message[200];
};


#endif
