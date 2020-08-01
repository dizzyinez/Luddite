#ifndef GUIElement_hpp
#define GUIElement_hpp

#include "rhea/variable.hpp"
#include "rhea/simplex_solver.hpp"
#include <vector>
// #include "gui/GuiState.hpp"

struct C_GUI
{
public:
        C_GUI()
        {
        }
        rhea::variable x, y, w, h;
};

//------------------------------------------//

struct C_GUI_Container
{
public:
        C_GUI_Container()
        {
        }
        rhea::simplex_solver solver;
        std::vector<std::unique_ptr<C_GUI> > children;
};

#endif
