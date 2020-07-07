#include "systems/GuiSystem.hpp"

#include "components/Size.hpp"
#include "components/Transform.hpp"
#include "components/Drawable.hpp"
#include "components/GUIElement.hpp"
// #include "components/Texture.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Locator.hpp"
#include <iostream>

#include "rhea/simplex_solver.hpp"

//TODO: add LUA integration
void GuiSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
        for (auto entity : es.entities_with_components<GUIElement, Size, Transform>())
        {
                entityx::ComponentHandle<GUIElement> gui = entity.component<GUIElement>();
                entityx::ComponentHandle<Size> size = entity.component<Size>();
                entityx::ComponentHandle<Transform> transform = entity.component<Transform>();
                entityx::ComponentHandle<Drawable> drawable = entity.component<Drawable>();

                // if (gui)
                // {
                //         if (gui->guistate != nullptr)
                //         {
                //                 // std::cout << gui->guistate->guicomponent << std::endl;
                //                 if (gui->guistate->guicomponent == 0)
                //                 {
                //                         gui->guistate->guicomponent = gui;
                //                         gui->guistate->initialize(&es);
                //                         gui->guistate->calculateChildPositions(windowSize.x, windowSize.y);
                //                         for (auto g : gui->guistate->children)
                //                         {
                //                                 if (g->guistate != nullptr)
                //                                 {
                //                                         g->guistate->guicomponent = gui;
                //                                         g->guistate->initialize(&es);
                //                                 }
                //                         }
                //                 }
                //                 // gui->guistate->initialize(&es);
                //         }
                // }

                if (gui && size && transform) {
                        transform->setX(gui->x.value());
                        transform->setY(gui->y.value());
                        size->setW(gui->w.value());
                        size->setH(gui->h.value());
                }
                if (drawable)
                {
                        drawable->layer = Layer::gui;
                }
        }

        rhea::simplex_solver solver;
        rhea::variable left, middle, right;
        //
        solver.add_constraints({
                middle == (left + right) / 2,
                right == left + 10,
                right <= 100,
                left >= 0
        });
        solver.suggest(middle, 20);
        // std::cout << left << std::endl;
        // delete &left;
        // std::cout << middle.value() << " " << right.value() << std::endl;

}

void GuiSystem::receive(const WindowResize &windowResize)
{
        // for (auto entity : Locator::getGame()->entities.entities_with_components<GUIElement, Size>()) //size might soon be redundant since I might make a size component mandatory for the guielement, but maybe there's some wierd exception I can't think of? IDK.
        // {
        //         entityx::ComponentHandle<GUIElement> gui = entity.component<GUIElement>();
        //         entityx::ComponentHandle<Size> size = entity.component<Size>();
        //
        //         if (gui && gui->guistate != nullptr)
        //         {
        //                 gui->guistate->calculateChildPositions(windowResize.x, windowResize.y);
        //
        //         }
        // }
}
