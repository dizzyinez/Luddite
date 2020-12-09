#ifndef S_Motion_hpp
#define S_Motion_hpp

#include "systems/System.hpp"

struct S_Motion : public System<S_Motion>
{
        void update(float deltaTime, entt::registry &reg);
};

#endif
