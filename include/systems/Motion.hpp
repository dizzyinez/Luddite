#ifndef Motion_hpp
#define Motino_hpp

#include "systems/System.hpp"

struct S_Motion : public System<S_Motion>
{
        void update(float deltaTime, entt::registry &reg) override;
};

#endif
