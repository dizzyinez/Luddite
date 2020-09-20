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
#include "ecs/Entity.hpp"
#include "script/PlayerScript.hpp"

#include <glm/glm.hpp>


#include "data/assets.hpp"
#include "data/resources.hpp"

void S_Spawning::update(float deltaTime, entt::registry &reg)
{
        Events::iterate<E_SpawnPlayer>([&reg, this](auto e){
                auto &slots = reg.ctx<C_PlayerSlots>();
                if (slots.players[e->slot] == entt::null)
                {
                        auto player = CreateEntity();
                        std::cout << "spawning" << std::endl;
                        slots.players[e->slot] = player.GetId();
                        player.AddScript<PlayerScript>();
                        player.AddComponent<C_Position>(e->xpos, e->ypos);
                        player.AddComponent<C_Velocity>();
                        player.AddComponent<C_Drag>(0.8f);
                        player.AddComponent<C_Size>(200.0f, 200.0f);
                        player.AddComponent<C_DrawLayer>();
                        // player.AddComponent<C_Sprite>(textures.Get("../assets/characters/character/character.png"), glm::uvec4(0xAAAABBFF, 0x555555FF, 0xAAAAAAFF, 0xFFFFFFFF)); //RGBA
                        player.AddComponent<C_Sprite>(textures.Get(Characters::GetTextureFilePath(Characters::eCharacter::character)), glm::uvec4(0xAAAABBFF, 0x555555FF, 0xAAAAAAFF, 0xFFFFFFFF)); //RGBA
                        player.AddComponent<C_Tileset>(50, 8, 0);
                        player.AddComponent<C_Animation>(6, 50, 0, 1.0/30.0, 0.0, true, true);
                        player.AddComponent<C_Player>(e->localPlayer, e->slot);
                        player.AddComponent<C_PlayerDirection>();
                        if (e->localPlayer)
                        {
                                player.AddComponent<C_PlayerKeymap>();
                        }
                        else
                        {
                                player.AddComponent<C_Net_Position>(e->xpos, e->ypos);
                        }
                }
                else
                {
                        // reg.replace<C_Position>(player, e->xpos, e->ypos);
                }
                return true;
        });
}
