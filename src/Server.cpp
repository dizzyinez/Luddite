#include "core/net.hpp"
#include "config.h"
#include "components/Player.hpp"
#include "components/Simulation.hpp"
#include "layers/L_MainMenu.hpp"
#include "layers/L_Game.hpp"

int Server::AddPlayer(short_string name, bool is_local)
{
        for (int i = 0; i < 256; i++)
        {
                Player &player = player_list[i];
                if (!player.exists)
                {
                        player.id = i;
                        player.exists = true;
                        player.local = is_local;
                        player.name = name;
                        return i;
                }
        }
        std::cout << "no room for more players" << std::endl;
        return -1;
}

void Server::AddPlayer(short_string name, ENetPeer* peer)
{
        int index = AddPlayer(name, false);
        if (index != -1)
        {
                std::cout << "new player, index is " << index << std::endl;
                GetData(peer)->index = index;
        }
}

bool Server::OnClientConnect(ENetPeer* peer)
{
        return true;
}

void Server::OnClientDisconnect(ENetPeer* peer)
{
}

void Server::OnMessage(Message& msg, ENetPeer* peer)
{
        switch (msg.header.id)
        {
        case message_types::CONNECTION_REQUEST:
                int version_major;
                int version_minor;
                msg >> version_minor >> version_major;
                std::cout << "connection requested, version: " << version_major << "." << version_minor << std::endl;
                if (version_major == VERSION_MAJOR && version_minor == VERSION_MINOR)
                {
                        //approve the connnection
                        Message msg_approve;
                        msg_approve.header.id = message_types::CONNECTION_APPROVED;
                        MessageClient(msg_approve, peer);

                        //send them the existing players
                        for (Player p : player_list)
                        {
                                if (p.exists)
                                {
                                        Message msg_player;
                                        msg_player.header.id = message_types::NEW_PLAYER;
                                        msg_player << p.name << p.id;
                                        MessageClient(msg_player, peer);
                                }
                        }
                }
                else
                {
                        //deny the connection
                        Message out;
                        out.header.id = message_types::CONNECTION_DENIED;
                        MessageClient(out, peer);
                }
                break;

        case message_types::NEW_PLAYER:
        {
                //read packet
                short_string name;
                msg >> name;
                std::cout << "new player connected" << std::endl;
                AddPlayer(name, peer);

                //acknowledge the new player
                Message out;
                out.header.id = message_types::PLAYER_ACK;
                out << name << GetData(peer)->index;
                MessageClient(out, peer);

                //send new player to other clients
        }
        break;

        case message_types::DISCONNECTION:
                std::cout << "client disconnected" << std::endl;
                break;

        case message_types::PLAYER_INPUT:
        {
                Message out = msg;
                Player &player = player_list[GetData(peer)->index];
                if (player.exists)
                {
                        auto& sf = l_game->GetContext<C_StoredFrames>();
                        std::cout << "size:" << msg.body.size() << std::endl;
                        uint64_t input_frame;
                        msg >> input_frame;
                        int index = sf.frame_id_to_index(input_frame);
                        if (index < 0 || index > sf.index)
                        {
                                std::cout << "invalid index" << std::endl;
                                break;
                        }
                        if (sf.frame_array.at(index).valid(player.entity.GetId()))
                                msg >> sf.frame_array.at(index).get<C_PlayerInput>(player.entity.GetId()).buttons;
                        else
                                std::cout << "FUCK" << std::endl;

                        out << GetData(peer)->index;
                        MessageAllClients(out);
                }
        }
        break;
        }
}