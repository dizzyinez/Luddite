#include "systems/networking.hpp"
#include "events/Events.hpp"
#include "events/networking.hpp"
#include "core/PacketDefinitions.hpp"
#include "components/Networking.hpp"
#include "components/Player.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"

std::string sstring_to_hex(const std::string& input)
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


void S_Net_Update_Player::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_Player>().each([&reg](auto Entity, auto &player) {
                if (player.local_player)
                {
                        // std::cout << "player.playerSlot: " << static_cast<int>(player.playerSlot) << std::endl;
                        auto &player_input = reg.get<C_PlayerInput>(Entity);
                        P_Player_Control p;
                        p.player_slot = player.player_slot;
                        p.buttons = player_input.buttons;
                        // p.x = pos.getX();
                        // p.y = pos.getY();
                        // P_Player_Control p(player.playerSlot, 0.0f, 0.0f);
                        // char haha[50];
                        // memcpy(&haha, convertToPacket(p)->data, convertToPacket(p)->dataLength);
                        // std::cout << sstring_to_hex(haha) << std::endl;
                        Events::emit<E_Net_Send>(convertToPacket(p));
                }
        });
}
