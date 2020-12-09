#ifndef S_Logging_hpp
#define S_Logging_hpp

#include "systems/System.hpp"

struct S_Logging : public System<S_Logging>
{
        void update(float deltaTime, entt::registry &reg);
};

#endif
