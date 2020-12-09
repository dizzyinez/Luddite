#ifndef Scripts_hpp
#define Scripts_hpp

#include "systems/System.hpp"

struct S_Scripts_Events : public System<S_Scripts_Events>
{
        void update(float deltaTime, entt::registry &reg);
};
struct S_Scripts_Update : public System<S_Scripts_Update>
{
        void update(float deltaTime, entt::registry &reg);
};
struct S_Scripts_LateUpdate : public System<S_Scripts_LateUpdate>
{
        void update(float deltaTime, entt::registry &reg);
};
#endif
