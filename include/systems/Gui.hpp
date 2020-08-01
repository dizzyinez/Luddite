#ifndef Gui_hpp
#define Gui_hpp

#include "systems/System.hpp"

struct S_Gui : public System<S_Motion>
{
        void update(float deltaTime, entt::registry &reg) override;
};

#endif
