#ifndef Draw_hpp
#define Draw_hpp

#include "systems/System.hpp"

struct S_Draw : public System<S_Draw>
{
        void update(float deltaTime, entt::registry &reg) override;
};

#endif
