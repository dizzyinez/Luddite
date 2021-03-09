#pragma once
#include "net/message.hpp"
#include "net/server.hpp"
#include "net/client.hpp"
#include "ecs/entity.hpp"
// #include "layers/L_MainMenu.hpp"
// #include "layers/L_Game.hpp"

enum class message_types : uint32_t
{
        CONNECTION_REQUEST,
        CONNECTION_APPROVED,
        CONNECTION_DENIED,
        NEW_PLAYER,
        PLAYER_ACK,
        START_GAME,
        PLAYER_INPUT,
        DISCONNECTION,
};

using short_string = std::array<char, 20>;

struct Player
{
        short_string name;
        int id = 0;
        bool exists = false;
        bool local = false;
        Entity entity;
};

typedef std::array<Player, 256> PlayerList;

struct ClientData
{
        bool spectating = false;
        uint8_t index;
};

using Message = net::message<message_types>;

class L_MainMenu;
class L_Game;
class Server : public net::server_interface<message_types, ClientData>
{
public:
        void AddLocalPlayer(short_string name) {AddPlayer(name, true);}
        L_MainMenu* l_main_menu;
        L_Game* l_game;
private:
        friend class L_Game;
        int AddPlayer(short_string name, bool is_local);
        int AddPeerPlayer(short_string name, ENetPeer* peer);
        bool OnClientConnect(ENetPeer* peer);
        void OnClientDisconnect(ENetPeer* peer);
        void OnMessage(Message& msg, ENetPeer* peer);
        PlayerList player_list;
};

class Client : public net::client_interface<message_types>
{
public:
        friend class L_Game;
        L_MainMenu* l_main_menu;
        L_Game* l_game;
private:
        void OnMessage(Message &msg);
        PlayerList player_list;
};