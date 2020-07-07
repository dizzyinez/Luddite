#ifndef GuiState_Hpp
#define GuiState_Hpp

#include <vector>
#include "rhea/simplex_solver.hpp"
#include "entityx/entityx.h"

#include "components/Transform.hpp"
#include "components/Size.hpp"
#include "components/Drawable.hpp"

struct GUIElement;

class GuiState
{
public:
rhea::simplex_solver solver;
entityx::ComponentHandle<GUIElement> guicomponent; //maybe make this a pointer?


std::vector<entityx::ComponentHandle<GUIElement> > children;
virtual void initialize(entityx::EntityManager* es)
{

}
virtual void calculateChildPositions(int windowW, int windowH)
{
}

virtual void onClick()
{
}

virtual void onHover()
{
}

virtual void setValue(float v)
{
}

entityx::ComponentHandle<GUIElement> Gui_CreatePanel(entityx::EntityManager* es)
{
        entityx::Entity gui = es->create();
        gui.assign<Transform>(0.0f, 0.0f);
        gui.assign<Size>(0.0f, 0.0f);
        gui.assign<Drawable>();
        this->children.push_back(gui.assign<GUIElement>());
        return gui.component<GUIElement>();
}
entityx::ComponentHandle<GUIElement> Gui_CreateEmpty(entityx::EntityManager* es)
{
        entityx::Entity gui = es->create();
        gui.assign<Transform>(0.0f, 0.0f);
        gui.assign<Size>(0.0f, 0.0f);
        this->children.push_back(gui.assign<GUIElement>());
        return gui.component<GUIElement>();
}
};

#endif
