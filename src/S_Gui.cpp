#include "systems/Gui.hpp"
#include "components/Gui.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"
#include "entt.hpp"

#include <iostream>

void on_gui_container_create(entt::registry &Reg, entt::entity Entity)
{
        Reg.get<C_Gui_Container>(Entity).onBirth(Reg.get<C_Gui>(Entity), Reg.get<C_Gui_Container>(Entity));//ugly!
}

void S_Gui::configure(entt::registry &reg)
{
        reg.on_construct<C_Gui_Container>().connect<&on_gui_container_create>();
}

void S_Gui::update(float deltaTime, entt::registry &reg)
{
        reg.group<C_Gui>(entt::get<C_Position, C_Size>).each([](auto Entity, auto &Gui, auto &Pos, auto &Size){
                Pos.setX(Gui.x.value());
                Pos.setY(Gui.y.value());
                Size.setW(Gui.w.value());
                Size.setH(Gui.h.value());
        });
}

void S_Gui_Input::update(float deltaTime, entt::registry &reg)
{
        reg.group<C_Gui_Container>(entt::get<C_Gui>).each([](auto Entity, auto &Gui_container, auto &Gui){
                Gui_container.onInput(Gui, Gui_container);
        });
}
