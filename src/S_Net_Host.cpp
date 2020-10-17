#include "systems/Networking.hpp"
#include "events/Logging.hpp"
#include "events/Networking.hpp"
#include "core/PacketDefinitions.hpp"
#include "components/Networking.hpp"

#include <iostream>

#include <string>
#include "net/server.hpp"

// std::string string_to_hex(const std::string& input)
// {
//         static const char hex_digits[] = "0123456789ABCDEF";
//
//         std::string output;
//         output.reserve(input.length() * 2);
//         for (unsigned char c : input)
//         {
//                 output.push_back(hex_digits[c >> 4]);
//                 output.push_back(hex_digits[c & 15]);
//         }
//         return output;
// }

enum class MsgTypes : uint32_t
{
        PlayerInput,
        Chat
};


void S_Net_Host::update(float deltaTime, entt::registry &reg)
{
        auto &server = reg.ctx<C_Net_Server>();
        // net::message<MsgTypes> msg;
        // msg.header.id = MsgTypes::PlayerInput;
}
