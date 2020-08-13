#include "systems/Spawning.hpp"
#include "events/Events.hpp"
#include "events/Gameworld.hpp"
#include "components/Networking.hpp"
#include "components/Player.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Drag.hpp"
#include "components/Size.hpp"
#include "components/DrawLayer.hpp"
#include "components/PlayerKeymap.hpp"

void S_Spawning::update(float deltaTime, entt::registry &reg)
{
        Events::iterate<E_SpawnPlayer>([&reg](auto e){
                auto &slots = reg.ctx<C_PlayerSlots>();
                if (slots.players[e->slot] == entt::null)
                {
                        auto player = reg.create();
                        reg.emplace<C_Position>(player, e->xpos, e->ypos);
                        reg.emplace<C_Velocity>(player);
                        reg.emplace<C_Drag>(player, 0.8f);
                        reg.emplace<C_Size>(player, 150.0f, 150.0f);
                        reg.emplace<C_DrawLayer>(player);
                        reg.emplace<C_Player>(player, e->localPlayer, e->slot);
                        if (e->localPlayer)
                        {
                                reg.emplace<C_PlayerKeymap>(player);
                        }
                }
                else
                {
                        // reg.replace<C_Position>(player, e->xpos, e->ypos);
                }
                return true;
        });
}
