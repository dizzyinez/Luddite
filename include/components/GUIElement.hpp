#ifndef GUIElement_hpp
#define GUIElement_hpp
#include "entityx/entityx.h"
#include "rhea/variable.hpp"
// #include "gui/GuiState.hpp"

struct GUIElement : entityx::Component<GUIElement>
{
public:
        GUIElement()
        {
        }
        // GuiState* guistate; //fuck ecs for gui, I'm putting
        //                     //copious amounts of behavior in a
        //                     //component and NOBODY can stop me!
        rhea::variable x, y, w, h;
};

#endif
