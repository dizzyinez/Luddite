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
#include "components/Texture.hpp"
#include "components/Tileset.hpp"

#include <glm/glm.hpp>
#include "core/assets.hpp"

void S_Spawning::update(float deltaTime, entt::registry &reg)
{
        Events::iterate<E_SpawnPlayer>([&reg](auto e){
                auto &slots = reg.ctx<C_PlayerSlots>();
                if (slots.players[e->slot] == entt::null)
                {
                        std::cout << "spawning" << std::endl;
                        auto player = reg.create();
                        slots.players[e->slot] = player;
                        reg.emplace<C_Position>(player, e->xpos, e->ypos);
                        reg.emplace<C_Velocity>(player);
                        reg.emplace<C_Drag>(player, 0.8f);
                        reg.emplace<C_Size>(player, 200.0f, 200.0f);
                        reg.emplace<C_DrawLayer>(player);
                        reg.emplace<C_Sprite>(player, Assets::Sprite::Get(Assets::Sprite::player), glm::uvec4(0xFFFF00FF, 0xEEEEEEFF, 0xAAAAAAFF, 0xFFFFFFFF)); //RGBA
                        reg.emplace<C_Tileset>(player, 22, 8, 0);
                        reg.emplace<C_Animation>(player, 2, 22, 0, 1.0/30.0, 0.0, true, true);
                        reg.emplace<C_Player>(player, e->localPlayer, e->slot);
                        if (e->localPlayer)
                        {
                                reg.emplace<C_PlayerKeymap>(player);
                        }
                        else
                        {
                                reg.emplace<C_Net_Position>(player, e->xpos, e->ypos);
                        }
                }
                else
                {
                        // reg.replace<C_Position>(player, e->xpos, e->ypos);
                }
                return true;
        });
}
