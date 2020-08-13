#include "core/PacketDefinitions.hpp"
#include "events/events.hpp"
#include "events/networking.hpp"
#include "events/Gameworld.hpp"
#include "components/networking.hpp"
#include "components/player.hpp"
#include "components/position.hpp"

std::string string_to_hex(const std::string& input)
{
        static const char hex_digits[] = "0123456789ABCDEF";

        std::string output;
        output.reserve(input.length() * 2);
        for (unsigned char c : input)
        {
                output.push_back(hex_digits[c >> 4]);
                output.push_back(hex_digits[c & 15]);
        }
        return output;
}

void P_Connect_Request::on_host_receive(ENetPeer* peer, entt::registry &reg)
{
        P_Connect_Request rq;
        if (strcmp(version, rq.version) == 0) //0 is means they match in strcmp
        {
                auto &host = reg.ctx<C_Net_Host>();
                int slot = 0;
                auto &slots = reg.ctx<C_PlayerSlots>();
                for (int i = 0; i <= 256; i++)
                {
                        if (slots.players[i] == entt::null)
                        {
                                slot = i;
                                break;
                        }
                }
                slot = 4;
                if (slot != 256)
                {
                        P_Connect_Approved p(slot);
                        ENetPacket* packet = convertToPacket(p);
                        enet_peer_send(peer, 0, packet);
                        P_Spawn_Player sp(slot);
                        Events::emit<E_Net_Send>(convertToPacket(sp));
                        reg.group<C_Player>(entt::get<C_Position>).each([](auto Entity, auto &player, auto &pos){
                                P_Spawn_Player spawn(player.playerSlot, pos.getX(), pos.getY());
                                Events::emit<E_Net_Send>(convertToPacket(spawn));

                        });
                        Events::emit<E_SpawnPlayer>(slot, 0.0f, 0.0f);
                        /*
                           send packets to player to spawn all the other players
                         */
                        // enet_host_broadcast(host.server, 0, packet);
                }
                std::cout << std::endl;
        }
}

void P_Connect_Approved::on_client_receive(entt::registry &reg)
{
        reg.view<C_Player>().each([=](auto Entity, auto &player){
                player.playerSlot = Playerid;
        });
        std::cout << "Connection approved" <<std::endl;
}

void P_Spawn_Player::on_client_receive(entt::registry &reg)
{
        Events::emit<E_SpawnPlayer>(slot, x, y);
}
