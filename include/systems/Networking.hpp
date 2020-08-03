#ifndef S_Networking
#define S_Networking

#include "systems/Systems.hpp"

struct S_Net_Client : public System<S_Net_Client>
{
        void configure(entt::registry &reg) override;
        void update(float deltaTime, entt::registry &reg) override;
};

#endif
