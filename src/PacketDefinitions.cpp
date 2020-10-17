#include "core/PacketDefinitions.hpp"
#include "events/events.hpp"
#include "events/networking.hpp"
#include "events/Gameworld.hpp"
#include "components/networking.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
/*
 *
 * void P_Connect_Request::on_host_receive(ENetPeer* peer, entt::registry &reg)
 * {
 *      std::cout << "connection request is being handled, version: " << version << std::endl;
 *      P_Connect_Request rq;
 *      if (strcmp(version, rq.version) == 0) //0 is means they match in strcmp
 *      {
 *              auto &host = reg.ctx<C_Net_Host>();
 *              int slot = 256;
 *              auto &slots = reg.ctx<C_PlayerSlots>();
 *              for (int i = 0; i < 256; i++)
 *              {
 *                      if (slots.players[i] == entt::null)
 *                      {
 *                              slot = i;
 *                              break;
 *                      }
 *              }
 *              std::cout << "slot: " << slot << std::endl;
 *              if (slot != 256)
 *              {
 *                      P_Connect_Approved p;
 *                      p.player_slot = static_cast<uint8_t>(slot);
 *                      ENetPacket * packet = convertToPacket(p);
 *                      enet_peer_send(peer, 0, packet);
 *                      P_Spawn_Player spawn_peer;
 *                      spawn_peer.player_slot = slot;
 *                      Events::emit<E_Net_Send>(convertToPacket(spawn_peer));
 *                      reg.group<C_Player>(entt::get<C_Position>).each([](auto Entity, auto &player, auto &pos) {
 *                              P_Spawn_Player spawn;
 *                              spawn.player_slot = player.player_slot;
 *                              // spawn.x = pos.getX();
 *                              // spawn.y = pos.getY();
 *                              Events::emit<E_Net_Send>(convertToPacket(spawn));
 *                      });
 *                      std::cout << "emitting spawning event" << std::endl;
 *                      // Events::emit<E_SpawnPlayer>(1, 0, 0);
 *                      Events::emit<E_SpawnPlayer>(slot, 0.0f, 0.0f);
 *                      // enet_host_broadcast(host.server, 0, packet);
 *                      std::cout << "successfully handled connection request" << std::endl;
 *              }
 *      }
 *      else
 *      {
 *              P_Connect_Denied p;
 *              strcpy(p.version, rq.version);
 *              Events::emit<E_Net_Send>(convertToPacket(p));
 *      }
 * }
 *
 * void P_Connect_Approved::on_client_receive(entt::registry &reg)
 * {
 *      reg.view<C_Player>().each([&reg, this](auto Entity, auto &player) {
 *              auto &slots = reg.ctx<C_PlayerSlots>();
 *              player.player_slot = player_slot;
 *              slots.players[0] = entt::null;
 *              slots.players[player_slot] = Entity;
 *              std::cout << "player slot set to: " << player_slot << std::endl;
 *      });
 *      std::cout << "Connection approved" << std::endl;
 * }
 *
 * void P_Spawn_Player::on_client_receive(entt::registry &reg)
 * {
 *      std::cout << "emitting spawning event" << std::endl;
 *      Events::emit<E_SpawnPlayer>(player_slot, 0.0f, 0.0f);
 * }
 *
 * void P_Player_Control::on_host_receive(ENetPeer* peer, entt::registry &reg)
 * {
 *      auto &slots = reg.ctx<C_PlayerSlots>();
 *      if (player_slot <= 256 && slots.players[player_slot] != entt::null)
 *      {
 *              auto &player = reg.get<C_Player>(slots.players[player_slot]);
 *              if (player.local_player)
 *              {
 *                      // auto &pos = reg.get<C_Position>(slots.players[player_slot]);
 *                      // pos.setX(x);
 *                      // pos.setY(y);
 *              }
 *              else
 *              {
 *                      auto &player_input = reg.get<C_PlayerInput>(slots.players[player_slot]);
 *                      player_input.buttons = buttons;
 *                      // auto &net_pos = reg.get<C_Net_Position>(slots.players[player_slot]);
 *                      // net_pos.setX(x);
 *                      // net_pos.setY(y);
 *                      // net_pos.time = 0.04;
 *              }
 *      }
 * }
 *
 * void P_Player_Control::on_client_receive(entt::registry &reg)
 * {
 *      auto &slots = reg.ctx<C_PlayerSlots>();
 *      if (player_slot <= 256 && slots.players[player_slot] != entt::null)
 *      {
 *              auto &player = reg.get<C_Player>(slots.players[player_slot]);
 *              if (player.local_player)
 *              {
 *                      // auto &pos = reg.get<C_Position>(slots.players[player_slot]);
 *                      // pos.setX(x);
 *                      // pos.setY(y);
 *              }
 *              else
 *              {
 *                      auto &player_input = reg.get<C_PlayerInput>(slots.players[player_slot]);
 *                      player_input.buttons = buttons;
 *                      // auto &net_pos = reg.get<C_Net_Position>(slots.players[player_slot]);
 *                      // net_pos.setX(x);
 *                      // net_pos.setY(y);
 *                      // net_pos.time = 0.04;
 *              }
 *      }
 * }*/
