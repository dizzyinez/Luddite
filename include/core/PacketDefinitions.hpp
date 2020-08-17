#ifndef PacketDefinitions_hpp
#define PacketDefinitions_hpp

#include "entt.hpp"
#include <enet/enet.h>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <iostream>


template <typename T>
ENetPacket* convertToPacket(T& p)
{
        uint8_t data[sizeof(T)];
        size_t size = p.serialize(data);
        // data[0] = p.signature;
        // std::cout << "data to send: " << "    " << data << std::endl;
        return enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
};

template <typename T>
T convertFromPacket(ENetPacket* packet)
{
        T data;
        data.deserialize(packet->data);
        // std::cout << (!status) << std::endl;
        return data;

        //this probably causes a memory leak when
        //the data doesn't have a char array in it.
        //I couldn't find another way to do this
        //tho, so this is what we get.
        // strcpy(static_cast<char*>(static_cast<void*>(&data)), static_cast<char*>(static_cast<void*>(event.packet->data)));
        // memcpy(&data, event.packet->data, sizeof(T));

}

struct P_Fatal_Error
{
        // P_Fatal_Error() = default;
        // P_Fatal_Error(char* error_message_)
        // {
        //         strcpy(error_message, error_message_);
        // }


        uint8_t signature = 1;
        char error_message[100];
        size_t serialize(uint8_t* data)
        {
                data[0] = signature;
                memcpy(&data[1], &error_message, sizeof(error_message)); //strcpy?
                return 101;
        }
        void deserialize(uint8_t* data)
        {
                signature = data[0];
                memcpy(&error_message, &data[1], sizeof(error_message)); //strcpy?
        }
};
// inline constexpr auto properties(blob::tag<P_Fatal_Error>) {
//         auto props = blob::properties_t<P_Fatal_Error> { };
//
//         props.member<&P_Fatal_Error::signature>().expected_value = uint8_t { 1 };
//
//         return props;
// }


struct P_Connect_Request
{
        void on_host_receive(ENetPeer* peer, entt::registry &reg);
        char version[20] = "Luddite001";
        uint8_t signature = 2;

        size_t serialize(uint8_t* data)
        {
                data[0] = signature;
                memcpy(&data[1], &version, sizeof(version)); //strcpy?
                return 21;
        }
        void deserialize(uint8_t* data)
        {
                signature = data[0];
                memcpy(&version, &data[1], sizeof(version)); //strcpy?
        }
};

struct P_Connect_Approved
{
        void on_client_receive(entt::registry &reg);
        // P_Connect_Approved() = default;
        // P_Connect_Approved(uint8_t playerSlot_)
        //         : playerSlot(playerSlot_)
        // {
        // }
        // uint32_t player_slott = 0;
        uint8_t signature = 3;
        uint8_t player_slot;
        float x = 0.0f;
        float y = 0.0f;

        size_t serialize(uint8_t* data)
        {
                data[0] = signature;
                data[1] = player_slot;
                memcpy(&data[2], &x, 4);
                memcpy(&data[6], &y, 4);
                return 10;
        }
        void deserialize(uint8_t* data)
        {
                signature = data[0];
                player_slot = data[1];
                memcpy(&x, &data[2], 4);
                memcpy(&y, &data[6], 4);
        }
};

struct P_Connect_Denied
{
        // P_Connect_Denied() = default;
        char version[20];
        uint8_t signature = 4;
        // void on_client_receive(entt::registry &reg);
        size_t serialize(uint8_t* data)
        {
                data[0] = signature;
                memcpy(&data[1], &version, sizeof(version)); //strcpy?
                return 21;
        }
        void deserialize(uint8_t* data)
        {
                signature = data[0];
                memcpy(&version, &data[1], sizeof(version)); //strcpy?
        }
};

struct P_Spawn_Player
{
        void on_client_receive(entt::registry &reg);
        // P_Spawn_Player() = default;
        // P_Spawn_Player(uint8_t slot_)
        //         : slot(slot_)
        // {
        // }
        // P_Spawn_Player(uint8_t slot_, float x_, float y_)
        //         : slot(slot_), x(x_), y(y_)
        // {
        // }
        float x = 0.0f;
        float y = 0.0f;
        uint8_t signature = 5;
        uint8_t player_slot;
        size_t serialize(uint8_t* data)
        {
                data[0] = signature;
                data[1] = player_slot;
                memcpy(&data[2], &x, 4);
                memcpy(&data[6], &y, 4);
                return 10;
        }
        void deserialize(uint8_t* data)
        {
                signature = data[0];
                player_slot = data[1];
                memcpy(&x, &data[2], 4);
                memcpy(&y, &data[6], 4);
        }
};

struct P_Player_Control
{
        void on_host_receive(ENetPeer* peer, entt::registry &reg);
        void on_client_receive(entt::registry &reg);
        // P_Player_Control() = default;
        // P_Player_Control(uint8_t playerSlot_, float xpos_, float ypos_)
        //         :  playerSlot(playerSlot_), xpos(xpos_), ypos(ypos_)
        // {
        // }
        float x;
        float y;
        uint8_t signature = 6;
        uint8_t player_slot;
        size_t serialize(uint8_t* data)
        {
                data[0] = signature;
                data[1] = player_slot;
                memcpy(&data[2], &x, 4);
                memcpy(&data[6], &y, 4);
                return 10;
        }
        void deserialize(uint8_t* data)
        {
                signature = data[0];
                player_slot = data[1];
                memcpy(&x, &data[2], 4);
                memcpy(&y, &data[6], 4);
        }
};


#endif
