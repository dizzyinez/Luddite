#ifndef Gui_MainMenu_Hpp
#define Gui_MainMenu_Hpp

#include "gui/GuiState.hpp"

class Gui_MainMenu : public GuiState
{
public:
Gui_MainMenu(entityx::EntityManager* es)
{
        // this->windowSized = true;
}
void initialize(entityx::EntityManager* es)
{
        auto test =  Gui_CreatePanel(es);
        solver.add_constraints({
                        guicomponent->x == 0,
                        guicomponent->y == 0,
                        // C_windowW,
                        // C_windowH,
                        // guicomponent->w == GUIElement::screenW,
                        // guicomponent->h == GUIElement::screenH,
                        test->x == guicomponent->x,
                        test->y == guicomponent->y,
                        test->w == guicomponent->w,
                        test->h == guicomponent->h
                        // box2.component<GUIElement>()->h == 1
                });
}
void calculateChildPositions(int windowW, int windowH)
{
        C_windowW = guicomponent->w == windowW;
        C_windowH = guicomponent->h == windowH;
        solver.solve();
}
void onClick()
{

}
void onHover()
{

}
void setValue(float v)
{

}
private:
rhea::constraint C_windowW;
rhea::constraint C_windowH;
};

#endif
