#include "layers/LayerMainMenu.hpp"
#include <iostream>

#include "Locator.hpp"

#include "systems/Draw.hpp"
#include "systems/Motion.hpp"
#include "systems/Gui.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Size.hpp"
#include "components/DrawLayer.hpp"
#include "components/Gui.hpp"
#include "components/Networking.hpp"

#include "events/Events.hpp"
#include "events/Logging.hpp"
#include "events/Input.hpp"
#include "events/Networking.hpp"

#include <iostream>


#include "kiwi/kiwi.h"
// #include "rhea/simplex_solver.hpp "
// #include "gui/G_MainMenu.hpp "


void L_MainMenu::init()
{
        systems.add<S_Draw>();
        systems.add<S_Motion>();
        systems.add<S_Gui_Input>();
        systems.add<S_Gui>();
        systems.configure(m_Registry);

        auto entity = createEntity();
        m_Registry.emplace<C_Position>(entity, 0.0f, 0.0f);
        m_Registry.emplace<C_Velocity>(entity, 11.1f, 11.1f);
        m_Registry.emplace<C_Size>(entity, 150.0f, 150.0f);
        m_Registry.emplace<C_DrawLayer>(entity);

        auto gui = createEntity();
        m_Registry.emplace<C_Position>(gui);
        m_Registry.emplace<C_Size>(gui);
        m_Registry.emplace<C_DrawLayer>(gui, DrawLayer::gui);
        m_Registry.emplace<C_Gui>(gui);
        m_Registry.emplace<C_Gui_Container>(gui,
                                            [](auto &Gui, auto &Gui_container){
                Gui_container.solver->addEditVariable(Gui.x, kiwi::strength::strong);
                Gui_container.solver->addEditVariable(Gui.w, kiwi::strength::strong);
                Gui_container.solver->addEditVariable(Gui.h, kiwi::strength::strong);
                kiwi::Constraint constraints[] = {
                        kiwi::Constraint {Gui.x == 0},
                        kiwi::Constraint {Gui.y == 0}
                };
                for (auto& constraint : constraints)
                        Gui_container.solver->addConstraint(constraint);
                Gui_container.solver->updateVariables();
        },

                                            [](auto &Gui, auto &Gui_container){
                Events::iterateAll<E_WindowResize>([&Gui, &Gui_container](auto e){
                        Gui_container.solver->suggestValue(Gui.x, e->width / 5);
                        Gui_container.solver->suggestValue(Gui.w, e->width / 2);
                        Gui_container.solver->suggestValue(Gui.h, e->height / 2);
                        Gui_container.solver->updateVariables();
                        return false;
                });
        });

        auto client = createEntity();
        m_Registry.emplace<C_Net_Client>(client, 1, 1, 0, 0);

}

void L_MainMenu::handleEvents(float deltaTime)
{
        systems.update<S_Gui_Input>(deltaTime, m_Registry);
}
void L_MainMenu::update(float deltaTime)
{
        systems.update<S_Motion>(deltaTime, m_Registry);
        systems.update<S_Gui>(deltaTime, m_Registry);
}

void L_MainMenu::render(float deltaTime)
{
        systems.update<S_Draw>(deltaTime, m_Registry);
}
void L_MainMenu::clean()
{

}

L_MainMenu::L_MainMenu()
{

}
