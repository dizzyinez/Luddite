#pragma once

#include "kiwi/kiwi.h"
#include "entt.hpp"
#include <vector>
#include <functional>
#include <string>
#include "gui/constraints.hpp"
// #include "gui/GuiState.hpp"

struct Transition
{
        float time;
        bool change_interactable = false;
        bool interactable;
        float x_offset = 0;
        float y_offset = 0;
        float w_offset = 0;
        float h_offset = 0;
        float alpha_to = 1;
        dimensions x_dimension_coefficient = dimensions::NONE;
        dimensions y_dimension_coefficient = dimensions::NONE;
        dimensions w_dimension_coefficient = dimensions::NONE;
        dimensions h_dimension_coefficient = dimensions::NONE;
        Transition(float time_) : time(time_) {}
        Transition& set_x_offset(float offset, dimensions d = dimensions::NONE) {x_offset = offset; x_dimension_coefficient = d; return *this;}
        Transition& set_y_offset(float offset, dimensions d = dimensions::NONE) {y_offset = offset; y_dimension_coefficient = d; return *this;}
        Transition& set_w_offset(float offset, dimensions d = dimensions::NONE) {w_offset = offset; w_dimension_coefficient = d; return *this;}
        Transition& set_h_offset(float offset, dimensions d = dimensions::NONE) {h_offset = offset; h_dimension_coefficient = d; return *this;}
        Transition& set_alpha(float alpha) {alpha_to = alpha; return *this;}
        Transition& set_interactable(bool interactable_) {change_interactable = true; interactable = interactable_; return *this;}
};

struct C_Gui
{
        kiwi::Variable x = ("x_position");
        kiwi::Variable y = ("y_position");
        kiwi::Variable w = ("width");
        kiwi::Variable h = ("height");
        kiwi::Variable xd = ("x_offset");
        kiwi::Variable yd = ("y_offset");
        kiwi::Variable wd = ("w_offset");
        kiwi::Variable hd = ("h_offset");
        Transition current_transition = Transition(0);
        bool interactable = true;
        float lerp_time = 0;
        bool dirty_constraints = false;
        entt::entity parent = entt::null;
        Constraint constraints[4] = {{constraints::NONE, edges::CENTER, dimensions::WIDTH, 0.5f},
                {constraints::NONE, edges::CENTER, dimensions::WIDTH, 0.5f},
                {constraints::NONE, edges::CENTER, dimensions::WIDTH, 0.5f},
                {constraints::NONE, edges::CENTER, dimensions::WIDTH, 0.5f}};

        float text_scale_coefficient = 1.0f;
        dimensions text_scale_dimension = dimensions::NONE;
        bool dirty_text_scale = false;

        C_Gui() = default;
        // C_Gui(entt::entity parent_) : parent(parent_) {}
        C_Gui& set_parent(entt::entity parent_) {parent = parent_; return *this;}
        C_Gui& set_x(Constraint c) {constraints[0] = c; dirty_constraints = true; return *this;}
        C_Gui& set_y(Constraint c) {constraints[1] = c; dirty_constraints = true; return *this;}
        C_Gui& set_w(Constraint c) {constraints[2] = c; dirty_constraints = true; return *this;}
        C_Gui& set_h(Constraint c) {constraints[3] = c; dirty_constraints = true; return *this;}
        C_Gui& set_text_scale(float s, dimensions d) {text_scale_coefficient = s; text_scale_dimension = d; dirty_text_scale = true; return *this;}

        C_Gui& transition(const Transition& transition)
        {
                current_transition = transition;
                lerp_time = transition.time;
                if (transition.change_interactable)
                {
                        set_interactable(transition.interactable);
                }
                return *this;
        }
        C_Gui& set_interactable(bool interactable_)
        {
                interactable = interactable_;
                return *this;
        }
        void transition_reset(float time, bool interactable_ = true)
        {
                transition(Transition(time).set_interactable(interactable_));
        }
};

//------------------------------------------//

struct C_Gui_Container
{
        C_Gui_Container()
        {
        }
        C_Gui_Container(std::function<void(C_Gui&, C_Gui_Container&)> onBirth_)
                : onBirth(onBirth_)
        {
        }
        // C_Gui_Container(std::function<void(C_Gui&, C_Gui_Container&)> onBirth_, std::function<void(C_Gui&, C_Gui_Container&)> onInput_)
        //         : onBirth(onBirth_), onInput(onInput_)
        // {
        // }
        // C_Gui_Container(std::function<void(C_Gui&, C_Gui_Container&)> onBirth_, std::function<void(C_Gui&, C_Gui_Container&)> onInput_, std::function<void(C_Gui&, C_Gui_Container&)> onUpdate_)
        //         : onBirth(onBirth_), onInput(onInput_), onUpdate(onUpdate_)
        // {
        // }
        C_Gui_Container& on_birth(std::function<void(C_Gui&, C_Gui_Container&)> on_birth_)
        {
                onBirth = on_birth_;
                return *this;
        }
        C_Gui_Container& on_input(std::function<void(C_Gui&, C_Gui_Container&)> on_input_)
        {
                onInput = on_input_;
                return *this;
        }
        C_Gui_Container& on_update(std::function<void(C_Gui&, C_Gui_Container&)> on_update_)
        {
                onUpdate = on_update_;
                return *this;
        }
        std::vector<std::unique_ptr<C_Gui> > children;
        std::function<void(C_Gui&, C_Gui_Container&)> onBirth = [](auto &Gui, auto &Gui_container) {};
        std::function<void(C_Gui&, C_Gui_Container&)> onInput = [](auto &Gui, auto &Gui_container) {};
        std::function<void(C_Gui&, C_Gui_Container&)> onUpdate = [](auto &Gui, auto &Gui_container) {};
        std::unique_ptr<kiwi::Solver> solver = std::make_unique<kiwi::Solver>();
};

struct C_Gui_Clickable
{
        C_Gui_Clickable() = default;
        C_Gui_Clickable& on_hover_over(std::function<void(C_Gui&, C_Gui_Clickable&)> onHoverOver_)
        {
                onHoverOver = onHoverOver_; return *this;
        }
        C_Gui_Clickable& on_hover_away(std::function<void(C_Gui&, C_Gui_Clickable&)> onHoverAway_)
        {
                onHoverAway = onHoverAway_; return *this;
        }
        C_Gui_Clickable& on_click(std::function<void(C_Gui&, C_Gui_Clickable&)> onClick_)
        {
                onClick = onClick_; return *this;
        }
        C_Gui_Clickable& reset_on_hover_away(float time, bool interactable_ = true)
        {
                onHoverAway = [time, interactable_](auto &Gui, auto &Gui_clickable) {Gui.transition_reset(time, interactable_);}; return *this;
        }


        bool hovering = false;
        std::function<void(C_Gui&, C_Gui_Clickable&)> onHoverOver = [](auto &Gui, auto &Gui_clickable) {};
        std::function<void(C_Gui&, C_Gui_Clickable&)> onHoverAway = [](auto &Gui, auto &Gui_clickable) {};
        std::function<void(C_Gui&, C_Gui_Clickable&)> onClick = [](auto &Gui, auto &Gui_clickable) {};
};

struct C_Gui_Textbox
{
        bool highlighted = false;
        uint32_t text_position = 0;
};