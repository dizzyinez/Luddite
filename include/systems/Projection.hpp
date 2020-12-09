#ifndef Projection_hpp
#define Projection_hpp

#include "systems/System.hpp"

struct S_Projection : public System<S_Projection>
{
        void update(float deltaTime, entt::registry &reg);
};

#endif
