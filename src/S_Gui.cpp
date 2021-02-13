#include "systems/Gui.hpp"
#include "components/Gui.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/Texture.hpp"
#include "components/Text.hpp"
#include "components/DrawLayer.hpp"
#include "components/Ancestry.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"
#include "entt.hpp"
#include "utils/lerp.hpp"
#include "core/game.hpp"

#include <iostream>
#include <GLFW/glfw3.h>

void on_gui_container_create(entt::registry &reg, entt::entity Entity)
{
        reg.get<C_Gui_Container>(Entity).onBirth(reg.get<C_Gui>(Entity), reg.get<C_Gui_Container>(Entity));
}

void S_Gui::on_gui_create(entt::registry &reg, entt::entity Entity)
{
        //set the Gui parent to the window if it doesn't have a parent
        reg.get<C_Gui>(Entity).parent = window_entity;
}

void on_text_create(entt::registry &reg, entt::entity Entity)
{
        C_Text& text = reg.get<C_Text>(Entity);
        text.cursor = reg.create();
        reg.emplace<C_Position>(text.cursor);
        reg.emplace<C_Size>(text.cursor);
        reg.emplace<C_Child>(text.cursor, Entity);
        reg.emplace<C_Tint>(text.cursor, glm::vec4(0, 0, 0, 1));
        C_DrawLayer& drawlayer = reg.get<C_DrawLayer>(Entity);
        reg.emplace<C_DrawLayer>(text.cursor, drawlayer.layer, drawlayer.depth + 1);
}

void S_Gui::configure(entt::registry &reg)
{
        reg.on_construct<C_Gui_Container>().connect<&on_gui_container_create>();
        reg.on_construct<C_Text>().connect<&on_text_create>();

        window_entity = reg.create();
        reg.emplace<C_Position>(window_entity, 0, 0);
        reg.emplace<C_Size>(window_entity);
        reg.emplace<C_Gui>(window_entity);
        reg.emplace<C_Gui_Container>(window_entity, [] (auto &Gui, auto &Gui_container) {
                Gui_container.solver->addEditVariable(Gui.w, kiwi::strength::strong);
                Gui_container.solver->addEditVariable(Gui.h, kiwi::strength::strong);
                Gui_container.solver->addConstraint({Gui.x == 0});
                Gui_container.solver->addConstraint({Gui.y == 0});
                Gui_container.solver->updateVariables();
                Gui_container.solver->suggestValue(Gui.w, Game::window_width);
                Gui_container.solver->suggestValue(Gui.h, Game::window_height);
        })
        .on_input([] (auto &Gui, auto &Gui_container) {
                Events::iterateAll<E_WindowResize>([&Gui, &Gui_container](auto e) {
                        Gui_container.solver->suggestValue(Gui.w, e->width);
                        Gui_container.solver->suggestValue(Gui.h, e->height);
                        Gui_container.solver->updateVariables();
                        return false;
                });
        });

        std::cout << "WINDOW INSTANTIANTED: " << (window_entity == entt::null) << std::endl;

        reg.on_construct<C_Gui>().connect<&S_Gui::on_gui_create>(this);
}

void S_Gui::update(float deltaTime, entt::registry &reg)
{
        reg.group<C_Gui>(entt::get<C_Position, C_Size>).each([this, &reg, deltaTime](auto Entity, C_Gui &Gui, auto &Pos, auto &Size) {
                //set the Gui parent to the window if it doesn't have a parent
                if (Gui.parent == entt::null) Gui.parent = window_entity;
                C_Gui_Container& gc = reg.get<C_Gui_Container>(Gui.parent);
                if (Gui.dirty_constraints)
                {
                        Gui.dirty_constraints = false;
                        gc.solver->addEditVariable(Gui.xd, kiwi::strength::strong);
                        gc.solver->addEditVariable(Gui.yd, kiwi::strength::strong);
                        gc.solver->addEditVariable(Gui.wd, kiwi::strength::strong);
                        gc.solver->addEditVariable(Gui.hd, kiwi::strength::strong);
                        for (int i = 0; i < 4; i++)
                        {
                                if (Gui.constraints[i].type == constraints::NONE)
                                {
                                        continue;
                                }
                                // std::cout << "constraint creation" << std::endl;
                                reg.get<C_Gui_Container>(Gui.parent).solver->addConstraint(create_constraint(Gui, reg.get<C_Gui>(Gui.parent), i) | kiwi::strength::medium);
                        }
                        C_Gui& Parent = reg.get<C_Gui>(Gui.parent);
                        reg.get<C_Gui_Container>(Gui.parent).solver->updateVariables();
                }
                if (Gui.lerp_time > 0)
                {
                        float alpha;
                        if (Gui.lerp_time <= deltaTime)
                        {
                                alpha = 1;
                                Gui.lerp_time = 0;
                        }
                        else
                        {
                                alpha = deltaTime / Gui.lerp_time;
                                Gui.lerp_time -= deltaTime;
                        }
                        Gui.lerp_time -= deltaTime;

                        C_Gui& Parent = reg.get<C_Gui>(Gui.parent);
                        //wow this is some wide fucking code
                        gc.solver->suggestValue(Gui.xd, Lerp((float)Gui.xd.value(), Gui.current_transition.x_dimension_coefficient != dimensions::NONE ? Gui.current_transition.x_dimension_coefficient == dimensions::HEIGHT ? Gui.current_transition.x_offset * (float)Parent.h.value() : Gui.current_transition.x_offset * (float)Parent.w.value() : Gui.current_transition.x_offset, alpha));
                        gc.solver->suggestValue(Gui.yd, Lerp((float)Gui.yd.value(), Gui.current_transition.y_dimension_coefficient != dimensions::NONE ? Gui.current_transition.y_dimension_coefficient == dimensions::HEIGHT ? Gui.current_transition.y_offset * (float)Parent.h.value() : Gui.current_transition.y_offset * (float)Parent.w.value() : Gui.current_transition.y_offset, alpha));
                        gc.solver->suggestValue(Gui.wd, Lerp((float)Gui.wd.value(), Gui.current_transition.w_dimension_coefficient != dimensions::NONE ? Gui.current_transition.w_dimension_coefficient == dimensions::HEIGHT ? Gui.current_transition.w_offset * (float)Parent.h.value() : Gui.current_transition.w_offset * (float)Parent.w.value() : Gui.current_transition.w_offset, alpha));
                        gc.solver->suggestValue(Gui.hd, Lerp((float)Gui.hd.value(), Gui.current_transition.h_dimension_coefficient != dimensions::NONE ? Gui.current_transition.h_dimension_coefficient == dimensions::HEIGHT ? Gui.current_transition.h_offset * (float)Parent.h.value() : Gui.current_transition.h_offset * (float)Parent.w.value() : Gui.current_transition.h_offset, alpha));
                        if (reg.has<C_Text>(Entity))
                        {
                                C_Text& text = reg.get<C_Text>(Entity);
                                if (Gui.text_scale_dimension != dimensions::NONE)
                                {
                                        text.recalculate_position_flag = true;
                                        text.set_pixel_scale(Gui.text_scale_dimension == dimensions::HEIGHT ? Gui.text_scale_coefficient * Gui.h.value() : Gui.text_scale_coefficient * Gui.w.value());
                                }
                        }

                        if (reg.has<C_Tint>(Entity))
                        {
                                float& alpha_channel = reg.get<C_Tint>(Entity).tint.a;
                                alpha_channel = Lerp(alpha_channel, Gui.current_transition.alpha_to, alpha);
                        }
                        gc.solver->updateVariables();
                        //TODO: alpha lerping
                }
                else if (Gui.dirty_text_scale && reg.has<C_Text>(Entity))
                {
                        Gui.dirty_text_scale = false;
                        C_Text& text = reg.get<C_Text>(Entity);
                        if (Gui.text_scale_dimension != dimensions::NONE)
                        {
                                text.recalculate_position_flag = true;
                                text.set_pixel_scale(Gui.text_scale_dimension == dimensions::HEIGHT ? Gui.text_scale_coefficient * Gui.h.value() : Gui.text_scale_coefficient * Gui.w.value());
                                // std::cout << "set text scale to "
                        }
                }

                Pos.setX(Gui.x.value());
                Pos.setY(Gui.y.value());
                Size.setW(Gui.w.value());
                Size.setH(Gui.h.value());
        });
}

void S_Gui_Input::update(float deltaTime, entt::registry &reg)
{
        reg.group<C_Gui_Container>(entt::get<C_Gui>).each([](auto Entity, auto &Gui_container, auto &Gui) {
                Gui_container.onInput(Gui, Gui_container);
        });
        Events::iterate<E_CursorMotion>([&reg](auto &e)
        {
                for (auto Entity : reg.view<C_Gui_Clickable>())
                {
                        auto [gui, clickable] = reg.get<C_Gui, C_Gui_Clickable>(Entity);
                        //make a collision function (/util/ ?)
                        if (gui.interactable)
                        {
                                if (gui.x.value() <= e->xpos &&
                                    gui.y.value() <= e->ypos &&
                                    gui.x.value() + gui.w.value() >= e->xpos &&
                                    gui.y.value() + gui.h.value() >= e->ypos)
                                {
                                        if (!clickable.hovering)
                                        {
                                                clickable.onHoverOver(gui, clickable);
                                                clickable.hovering = true;
                                        }
                                }
                                else
                                {
                                        if (clickable.hovering)
                                        {
                                                clickable.onHoverAway(gui, clickable);
                                                clickable.hovering = false;
                                        }
                                }
                        }
                }
                return false;
        });

        if (Events::get<E_WindowResize>()->size() > 0)
        {
                std::cout << "window size changed" << std::endl;
                reg.view<C_Gui>().each([](auto Entity, C_Gui& Gui) {
                        Gui.dirty_text_scale = true;
                });
                reg.group<C_Text>(entt::get<C_Gui>).each([](auto Entity, C_Text& text, C_Gui& Gui) {
                        text.recalculate_position_flag = true;
                });
        }

        Events::iterate<E_MouseButton>([&reg](auto &e) {
                if (e->button == GLFW_MOUSE_BUTTON_1 && e->action == GLFW_RELEASE)
                {
                        for (auto Entity : reg.view<C_Gui_Clickable>())
                        {
                                auto [clickable, gui] = reg.get<C_Gui_Clickable, C_Gui>(Entity);
                                if (clickable.hovering)
                                {
                                        clickable.onClick(gui, clickable);
                                        return true;
                                }
                        }
                        // reg.group<C_Text>(entt::get<C_Gui>).each([&reg](auto Entity, C_Text & text, C_Gui & gui)
                        for (auto Entity : reg.group<C_Text>(entt::get<C_Gui>))
                        {
                                auto [text, gui] = reg.get<C_Text, C_Gui>(Entity);
                                if (gui.x.value() <= e->xpos &&
                                    gui.y.value() <= e->ypos &&
                                    gui.x.value() + gui.w.value() >= e->xpos &&
                                    gui.y.value() + gui.h.value() >= e->ypos)
                                {
                                        if (text.can_edit)
                                        {
                                                text.selected = true;
                                                if (text.text.size() == 0)
                                                {
                                                        text.cursor_position = 0;
                                                }
                                                else
                                                {
                                                        //set cursor position
                                                        C_Position& position = reg.get<C_Position>(Entity);
                                                        float adjusted_mouse_x_pos = (float)e->xpos - position.getX();
                                                        float adjusted_mouse_y_pos = (float)e->ypos - position.getY() - text.vertical_offset;

                                                        int line = floor(adjusted_mouse_y_pos / (128.0f * text.scale));
                                                        // check if the line the cursor is on actually exists
                                                        if (line < text.line_start_character_indicies.size())
                                                        {
                                                                //if it's the last line
                                                                if (line + 1 == text.line_start_character_indicies.size())
                                                                {
                                                                        C_Child& last_character_child = reg.get<C_Child>(text.character_entities.back());
                                                                        C_Size& last_character_size = reg.get<C_Size>(text.character_entities.back());
                                                                        float closest_distance = abs(adjusted_mouse_x_pos - (last_character_child.offset.x + last_character_size.getW()));
                                                                        int closest = text.character_entities.size();
                                                                        for (int i = closest - 1; i >= text.line_start_character_indicies.at(line); i--)
                                                                        {
                                                                                C_Child& child = reg.get<C_Child>(text.character_entities.at(i));
                                                                                float distance = abs(adjusted_mouse_x_pos - child.offset.x);
                                                                                if (distance < closest_distance)
                                                                                {
                                                                                        closest_distance = distance;
                                                                                        closest = i;
                                                                                }
                                                                        }
                                                                        text.cursor_position = closest;
                                                                        std::cout << "selected: " << text.cursor_position << std::endl;
                                                                }
                                                                else
                                                                {
                                                                        C_Child& last_character_child = reg.get<C_Child>(text.character_entities.at(text.line_start_character_indicies.at(line + 1) - 1));
                                                                        float closest_distance = abs(adjusted_mouse_x_pos - last_character_child.offset.x);
                                                                        int closest = text.line_start_character_indicies.at(line + 1) - 1;
                                                                        for (int i = closest - 1; i >= text.line_start_character_indicies.at(line); i--)
                                                                        {
                                                                                C_Child& child = reg.get<C_Child>(text.character_entities.at(i));
                                                                                float distance = abs(adjusted_mouse_x_pos - child.offset.x);
                                                                                if (distance < closest_distance)
                                                                                {
                                                                                        closest_distance = distance;
                                                                                        closest = i;
                                                                                }
                                                                        }
                                                                        text.cursor_position = closest;
                                                                        std::cout << "selected: " << text.cursor_position << std::endl;
                                                                }
                                                                return true;
                                                        }
                                                        // C_Child& child = reg.get<C_Child>(text.character_entities.at(i));
                                                        // C_Size& size = reg.get<C_Size>(text.character_entities.at(i));
                                                        // float x_distance = child.offset.x - adjusted_mouse_x_pos;
                                                        // float y_distance = child.offset.y + (size.getH() / 2.0f) - adjusted_mouse_y_pos;
                                                        // float distance_squared = x_distance * x_distance + y_distance * y_distance;
                                                        // if (distance_squared < closest_distance_squared)
                                                        // {
                                                        //         closest_distance_squared = distance_squared;
                                                        //         closest = i;
                                                        // }
                                                }
                                        }
                                }
                                else
                                {
                                        text.selected = false;
                                }
                        }
                }
                return false;
        });
        Events::iterate<E_Typed>([&reg](auto &e) {
                for (auto Entity : reg.group<C_Text>(entt::get<C_Gui>))
                {
                        auto [text, gui] = reg.get<C_Text, C_Gui>(Entity);
                        if (text.selected && text.can_edit)
                        {
                                text.text.insert(text.text.begin() + text.cursor_position, e->codepoint);
                                text.cursor_position++;
                                text.dirty = true;
                                text.recalculate_position_flag = true;
                                return true;
                        }
                }
                return false;
        });
        Events::iterate<E_Keyboard>([&reg](auto &e) {
                if (e->key == GLFW_KEY_BACKSPACE && (e->action == GLFW_PRESS || e->action == GLFW_REPEAT))
                {
                        for (auto Entity : reg.group<C_Text>(entt::get<C_Gui>))
                        {
                                auto [text, gui] = reg.get<C_Text, C_Gui>(Entity);
                                if (text.selected && text.can_edit)
                                {
                                        if (text.cursor_position > 0)
                                        {
                                                text.text.erase(text.text.begin() + (text.cursor_position - 1));
                                                text.cursor_position--;
                                                text.dirty = true;
                                                text.recalculate_position_flag = true;
                                                return true;
                                        }
                                }
                        }
                }
                return false;
        });
}