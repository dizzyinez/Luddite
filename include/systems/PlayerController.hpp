#ifndef S_PlayerController_hpp
#define S_PlayerController_hpp

#include "systems/System.hpp"

struct S_PlayerController : public System<S_PlayerController>
{
        void update(float deltaTime, entt::registry &reg) override;
};

#endif
