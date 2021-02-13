#include "utils/spawning.hpp"

#include "components/Player.hpp"
#include "components/Position.hpp"
#include "components/Origin.hpp"
#include "components/Velocity.hpp"
#include "components/Drag.hpp"
#include "components/Size.hpp"
#include "components/DrawLayer.hpp"
#include "components/PlayerKeymap.hpp"
#include "components/Texture.hpp"
#include "components/Tileset.hpp"
#include "components/Animation.hpp"
#include "components/AnimationBehavior.hpp"
#include "components/CircleCollider.hpp"
#include "components/Simulation.hpp"
#include "ecs/Entity.hpp"
#include "script/PlayerScript.hpp"
#include "data/assets.hpp"
#include "data/JsonAllocator.hpp"
#include "data/TextureAllocator.hpp"

#include "data/TextureAllocator.hpp"
#include "data/FontAllocator.hpp"
namespace utils
{
Entity SpawnPlayer(Layer* L, uint8_t slot, bool local_player = false)
{
        Entity player = L->CreateEntity();
        player.AddComponent<C_Position>(0.0f, 0.0f);
        player.AddComponent<C_Velocity>();
        C_Size& size = player.AddComponent<C_Size>(200.0f, 200.0f);
        player.AddComponent<C_Origin>(size.size * glm::vec2(0.5f, 0.8f));
        player.AddComponent<C_CircleCollider>(20);
        player.AddComponent<C_DrawLayer>(DrawLayer::sprite);
        player.AddComponent<C_Sprite>(TextureAllocator::Get(Characters::GetTextureFilePath(Characters::eCharacter::ping_pong)), glm::uvec4(0xA38F8BFF, 0x545563FF, 0xDFBEABFF, 0xE77664FF));                 //RGBA
        // player.AddComponent<C_Texture>(FontAllocator::Get("../assets/fonts/comic.ttf")->characters['P'].texture);
        player.AddComponent<C_Simulation>();
        C_AnimationBehavior& ab = player.AddComponent<C_AnimationBehavior>(Characters::GetAnimationFilePath(Characters::eCharacter::ping_pong), Characters::GetAnimationBehaviorFilePath(Characters::eCharacter::ping_pong));
        player.AddComponent<C_AnimationBehaviorState>();
        player.AddComponent<C_Tileset>(ab.json->root("rows").toNumber(), ab.json->root("lines").toNumber(), 0);
        player.AddComponent<C_Animation>();
        player.AddComponent<C_Player>(local_player, slot);
        player.AddComponent<C_PlayerInput>();
        player.AddComponent<C_PlayerDirection>();
        player.AddScript<PlayerScript>();
        if (local_player)
        {
                player.AddComponent<C_PlayerKeymap>();
        }
        return player;
}
}