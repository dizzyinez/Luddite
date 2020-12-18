#pragma once
#include "script/NativeScript.hpp"
#include "events/events.hpp"
// #include "components/Networking.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

#include "components/Player.hpp"
#include "components/PlayerKeymap.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/Velocity.hpp"
#include "components/Tileset.hpp"
#include "components/Animation.hpp"

#include "rendering/renderer.hpp"
#include "events/Input.hpp"
#include "data/JsonAllocator.hpp"
#include "data/assets.hpp"



struct PlayerScript : public NativeScript
{
public:
        // PlayerScript() {}
        // ~PlayerScript() {}
        void OnCreate()
        {
                // std::cout << "Player script instantiated" << std::endl;
                // animation = json_manager.Get(Characters::GetAnimationFilePath(Characters::eCharacter::character));
                // GetComponent<C_Animation>().PlayAnimation("Run");
        }
        void OnDestroy()
        {
                std::cout << "script destruction" << std::endl;
        }
        void OnEvents(float deltaTime)
        {
                // Events::iterate<E_CusrsorMotion>([this](auto &e) {
                //         glm::vec2 position = Renderer::screenToWorld(glm::vec2(e->xpos, e->ypos));
                //         // std::cout << glm::to_string(position) << std::endl;
                //         GetComponent<C_Position>().position.x = position.x - GetComponent<C_Size>().size.x / 2;
                //         GetComponent<C_Position>().position.y = position.y - GetComponent<C_Size>().size.y / 2;
                //         return false;
                // });
        }
        void OnUpdate(float deltaTime)
        {
                // std::cout << "native script updated" << std::endl;
                // std::cout << HasComponent<C_PlayerKeymap>() << std::endl;
        }
        void OnLateUpdate(float deltaTime)
        {
                // GetComponent<C_Animation>().direction = GetComponent<C_PlayerDirection>().movement_direction;
        }
};
