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
#include "data/resources.hpp"
#include "data/assets.hpp"

struct PlayerScript : public NativeScript
{
public:
        void OnCreate()
        {
                animation = json_manager.Get(Characters::GetAnimationFilePath(Characters::eCharacter::character));
                GetComponent<C_Animation>().PlayAnimation("Run");
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
        }
        void OnLateUpdate(float deltaTime)
        {
                // animation->root.child("animations")
                GetComponent<C_Animation>().direction = GetComponent<C_PlayerDirection>().movement_direction;
        }
private:
        std::shared_ptr<Json> animation;
};
