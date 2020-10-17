#include "systems/Networking.hpp"
#include "events/Events.hpp"
#include "events/Logging.hpp"
#include "events/Networking.hpp"
#include "core/PacketDefinitions.hpp"
#include "components/Networking.hpp"

#include <iostream>

#include <bitset>
#include <climits>

template<typename T>
void show_binrep(const T& a)
{
        const char* beg = reinterpret_cast<const char*>(&a);
        const char* end = beg + sizeof(a);
        while (beg != end)
                std::cout << std::bitset<CHAR_BIT>(*beg++) << ' ';
        std::cout << '\n';
}




void S_Net_Client::update(float deltaTime, entt::registry &reg)
{
        auto &client = reg.ctx<C_Net_Client>();
}
