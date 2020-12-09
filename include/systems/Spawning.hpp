#ifndef S_Spawning_hpp
#define S_Spawning_hpp

#include "systems/System.hpp"

struct S_Spawning : public System<S_Spawning>
{
        void update(float deltaTime, entt::registry &reg);
};

#endif
