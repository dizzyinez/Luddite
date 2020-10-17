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
                        auto &player_input = reg.get<C_PlayerInput>(Entity);
                        // p.player_slot = player.player_slot;
                        // p.buttons = player_input.buttons;
                }
        });
}
