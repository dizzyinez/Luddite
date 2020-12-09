#ifndef S_Networking
#define S_Networking

#include "systems/System.hpp"

struct S_Net_Client : public System<S_Net_Client>
{
        void update(float deltaTime, entt::registry &reg);
};

struct S_Net_Host : public System<S_Net_Host>
{
        void update(float deltaTime, entt::registry &reg);
};

struct S_Net_Send : public System<S_Net_Send>
{
        void update(float deltaTime, entt::registry &reg);
};

struct S_Net_Update_Player : public System<S_Net_Update_Player>
{
        void update(float deltaTime, entt::registry &reg);
};
#endif
