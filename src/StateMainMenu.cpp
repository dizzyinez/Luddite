#include "states/StateMainMenu.hpp"
#include <iostream>

#include "Locator.hpp"

#include "systems/Draw.hpp"
#include "systems/Motion.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Size.hpp"
#include "components/DrawLayer.hpp"
#include "components/Gui.hpp"

#include "events/Events.hpp"

// #include "rhea/simplex_solver.hpp "
// #include "gui/G_MainMenu.hpp "


void StateMainMenu::init()
{
        systems.add<S_Draw>();
        systems.add<S_Motion>();

        auto entity = createEntity();
        m_Registry.emplace<C_Position>(entity, 0.0f, 0.0f);
        m_Registry.emplace<C_Velocity>(entity, 21.1f, 21.1f);
        m_Registry.emplace<C_Size>(entity, 50.0f, 70.0f);
        m_Registry.emplace<C_DrawLayer>(entity);

        auto gui = createEntity();
        m_Registry.emplace<C_Position>(gui);
        m_Registry.emplace<C_Size>(gui);
        m_Registry.emplace<C_DrawLayer>(gui, DrawLayer::gui);
        m_Registry.emplace<C_GUI>(gui);
}

void StateMainMenu::handleEvents()
{

}
void StateMainMenu::update(float deltaTime)
{
        systems.update<S_Draw>(deltaTime, m_Registry);
        systems.update<S_Motion>(deltaTime, m_Registry);
}

void StateMainMenu::render(float deltaTime)
{

}
void StateMainMenu::clean()
{

}

StateMainMenu::StateMainMenu()
{

}
