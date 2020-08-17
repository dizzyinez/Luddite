#include "layers/LayerMainMenu.hpp"
#include <iostream>

#include "Locator.hpp"

#include "systems/Draw.hpp"
#include "systems/Motion.hpp"
#include "systems/Gui.hpp"
#include "systems/Networking.hpp"
#include "systems/PlayerController.hpp"
#include "systems/Spawning.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Drag.hpp"
#include "components/Size.hpp"
#include "components/DrawLayer.hpp"
#include "components/Gui.hpp"
#include "components/Networking.hpp"
#include "components/Player.hpp"
#include "components/PlayerKeymap.hpp"

#include "events/Events.hpp"
#include "events/Logging.hpp"
#include "events/Input.hpp"
#include "events/Networking.hpp"

#include <iostream>


#include "events/Gameworld.hpp"

void L_MainMenu::init()
{
        systems.add<S_Draw>();
        systems.add<S_Motion>();
        systems.add<S_Gui_Input>();
        systems.add<S_Gui>();
        systems.add<S_Net_Client>();
        systems.add<S_Net_Host>();
        systems.add<S_Net_Send>();
        systems.add<S_Net_Update_Player>();
        systems.add<S_PlayerController>();
        systems.add<S_Spawning>();
        systems.configure(m_Registry);



        Events::emit<E_SpawnPlayer>(true);

        auto host = createEntity();
        m_Registry.emplace<C_Position>(host);
        m_Registry.emplace<C_Size>(host);
        m_Registry.emplace<C_DrawLayer>(host, DrawLayer::gui);
        m_Registry.emplace<C_Gui>(host);
        m_Registry.emplace<C_Gui_Container>(host,
                                            [](auto &Gui, auto &Gui_container){
                Gui_container.solver->addEditVariable(Gui.w, kiwi::strength::strong);
                Gui_container.solver->addEditVariable(Gui.h, kiwi::strength::strong);
                kiwi::Constraint constraints[] = {
                        kiwi::Constraint {Gui.x == 0},
                        kiwi::Constraint {Gui.y == 0},
                        kiwi::Constraint {Gui.h <= 50},
                        kiwi::Constraint {Gui.w <= 50}
                };
                for (auto& constraint : constraints)
                        Gui_container.solver->addConstraint(constraint);
                Gui_container.solver->updateVariables();
        },

                                            [](auto &Gui, auto &Gui_container){
                Events::iterateAll<E_WindowResize>([&Gui, &Gui_container](auto e){
                        Gui_container.solver->suggestValue(Gui.w, e->width / 2);
                        Gui_container.solver->suggestValue(Gui.h, e->height / 2);
                        Gui_container.solver->updateVariables();
                        return false;
                });
        });
        m_Registry.emplace<C_Gui_Button>(host,
                                         [](){
                Events::emit<E_Net_Host>(27015, 32);
        });




        auto join = createEntity();
        m_Registry.emplace<C_Position>(join);
        m_Registry.emplace<C_Size>(join);
        m_Registry.emplace<C_DrawLayer>(join, DrawLayer::gui);
        m_Registry.emplace<C_Gui>(join);
        m_Registry.emplace<C_Gui_Container>(join,
                                            [](auto &Gui, auto &Gui_container){
                Gui_container.solver->addEditVariable(Gui.w, kiwi::strength::strong);
                Gui_container.solver->addEditVariable(Gui.h, kiwi::strength::strong);
                kiwi::Constraint constraints[] = {
                        kiwi::Constraint {Gui.x >= 55},
                        kiwi::Constraint {Gui.y == 0},
                        kiwi::Constraint {Gui.h <= 50},
                        kiwi::Constraint {Gui.w <= 50}
                };
                for (auto& constraint : constraints)
                        Gui_container.solver->addConstraint(constraint);
                Gui_container.solver->updateVariables();
        },

                                            [](auto &Gui, auto &Gui_container){
                Events::iterateAll<E_WindowResize>([&Gui, &Gui_container](auto e){
                        Gui_container.solver->suggestValue(Gui.w, e->width / 2);
                        Gui_container.solver->suggestValue(Gui.h, e->height / 2);
                        Gui_container.solver->updateVariables();
                        return false;
                });
        });
        m_Registry.emplace<C_Gui_Button>(join,
                                         [](){
                Events::emit<E_Net_Connect>("73.14.41.127", 27015);
        });


        // auto slots = createEntity();
        m_Registry.set<C_PlayerSlots>();

        // auto client = createEntity();
        m_Registry.set<C_Net_Client>();

        // auto hosst = createEntity();
        m_Registry.set<C_Net_Host>();

}

void L_MainMenu::handleEvents(float deltaTime)
{
        systems.update<S_Gui_Input>(deltaTime, m_Registry);
        systems.update<S_Net_Client>(deltaTime, m_Registry);
        systems.update<S_Net_Host>(deltaTime, m_Registry);
        systems.update<S_PlayerController>(deltaTime, m_Registry);
        systems.update<S_Spawning>(deltaTime, m_Registry);
}
void L_MainMenu::update(float deltaTime)
{
        systems.update<S_Motion>(deltaTime, m_Registry);
        systems.update<S_Gui>(deltaTime, m_Registry);


        systems.update<S_Net_Update_Player>(deltaTime, m_Registry);
        systems.update<S_Net_Send>(deltaTime, m_Registry);

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
