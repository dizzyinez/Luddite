#ifndef Gui_hpp
#define Gui_hpp

#include "systems/System.hpp"

struct S_Gui : public System<S_Gui>
{
        void configure(entt::registry &reg) override;
        void update(float deltaTime, entt::registry &reg);
        entt::entity window_entity = entt::null;
        void on_gui_create(entt::registry &Reg, entt::entity Entity);
};

struct S_Gui_Input : public System<S_Gui_Input>
{
        void update(float deltaTime, entt::registry &reg);
};


#endif
