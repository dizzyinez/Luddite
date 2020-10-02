#ifndef GuiElement_hpp
#define GuiElement_hpp

#include "kiwi/kiwi.h"
#include <vector>
#include <functional>
#include <string>
// #include "gui/GuiState.hpp"

struct C_Gui
{
public:
        C_Gui() = default;
        kiwi::Variable x = ("xpos");
        kiwi::Variable y = ("ypos");
        kiwi::Variable w = ("width");
        kiwi::Variable h = ("height");
};

//------------------------------------------//

struct C_Gui_Container
{
public:
        C_Gui_Container()
        {
        }
        C_Gui_Container(std::function<void(C_Gui&, C_Gui_Container&)> onBirth_)
                : onBirth(onBirth_)
        {
        }
        C_Gui_Container(std::function<void(C_Gui&, C_Gui_Container&)> onBirth_, std::function<void(C_Gui&, C_Gui_Container&)> onInput_)
                : onBirth(onBirth_), onInput(onInput_)
        {
        }
        C_Gui_Container(std::function<void(C_Gui&, C_Gui_Container&)> onBirth_, std::function<void(C_Gui&, C_Gui_Container&)> onInput_, std::function<void(C_Gui&, C_Gui_Container&)> onUpdate_)
                : onBirth(onBirth_), onInput(onInput_), onUpdate(onUpdate_)
        {
        }
        std::vector<std::unique_ptr<C_Gui> > children;
        std::function<void(C_Gui&, C_Gui_Container&)> onBirth = [](auto &Gui, auto &Gui_container) {};
        std::function<void(C_Gui&, C_Gui_Container&)> onInput = [](auto &Gui, auto &Gui_container) {};
        std::function<void(C_Gui&, C_Gui_Container&)> onUpdate = [](auto &Gui, auto &Gui_container) {};
        std::unique_ptr<kiwi::Solver> solver = std::make_unique<kiwi::Solver>();
};

struct C_Gui_Button
{
public:
        C_Gui_Button(std::function<void()> onClick_)
                : onClick(onClick_)
        {
        }
        //button timeout?
        std::function<void()> onClick;
};

#endif
