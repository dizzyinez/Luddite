#include "gui/G_MainMenu.hpp"
#include "components/GUIElement.hpp"

void GS_MainMenu::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
        for (auto entity : es.entities_with_components<GC_MainMenu>())
        {
                entityx::ComponentHandle<GC_MainMenu> gc = entity.component<GC_MainMenu>();
        }
}

void GS_MainMenu::receive(const WindowResize &windowResize)
{
        for (auto entity : es.entities_with_components<GC_MainMenu>())
        {
                entityx::ComponentHandle<GC_MainMenu> gc = event.component;
                entityx::ComponentHandle<GUIElement> guiE = entity.component<GUIElement>();
                gc->solver.suggest({
                        {guiE->w, windowResize.x},
                        {guiE->h, windowResize.y}
                });
        }
}

void GS_MainMenu::receive(const entityx::ComponentAddedEvent<GC_MainMenu> &event)
{
        auto entity = event.entity;
        entityx::ComponentHandle<GC_MainMenu> gc = event.component;
        entityx::ComponentHandle<GUIElement> guiE = entity.component<GUIElement>();
        gc->solver.add_constraints({
                guiE->x == 0,
                guiE->y == 0,
        });
}
