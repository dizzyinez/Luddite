#include "systems/networking.hpp"
#include "events/Events.hpp"
#include "events/networking.hpp"
#include "components/Networking.hpp"

void S_Net_Send::update(float deltaTime, entt::registry &reg)
{

        Events::iterate<E_Net_Send>([&reg](auto &e) {
                auto &client = reg.ctx<C_Net_Client>();
                if (client.initialized == true)
                        enet_peer_send(client.server, 0, e->packet);

                auto &host = reg.ctx<C_Net_Host>();
                if (host.initialized == true)
                        enet_host_broadcast(host.server, 0, e->packet);
                return true;
        });

        auto &client = reg.ctx<C_Net_Client>();
        if (client.initialized == true) {
                std::cout << client.server->roundTripTime << std::endl;
                enet_host_flush(client.client);
        }

        auto &host = reg.ctx<C_Net_Host>();
        if (host.initialized == true)
                enet_host_flush(host.server);
}
