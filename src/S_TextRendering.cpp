#include "systems/TextRendering.hpp"
#include "components/text.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/DrawLayer.hpp"
#include "components/Texture.hpp"
#include "components/Ancestry.hpp"

constexpr float LINE_SPACING = 128;

void S_Text_Rendering::update(float deltaTime, entt::registry &reg)
{
        auto view = reg.view<C_Text>();
        for (auto Entity : view)
        {
                auto &text = view.get<C_Text>(Entity);
                //get the font from the allocator
                if (text.dirty)
                {
                        text.dirty = false;

                        //delete any previous characters
                        for (entt::entity character_entity : text.character_entities)
                        {
                                reg.destroy(character_entity);
                                text.character_entities.clear();
                        }

                        //reserve the exact amount of space the vector needs so there's only one heap allocation
                        // text.character_entities.reserve(text.text.size());

                        //instantiate new characters
                        for (auto c = text.text.begin(); c != text.text.end(); c++)
                        {
                                // Character character;
                                // std::shared_ptr<Font> font;
                                std::shared_ptr<Character> character;
                                int i = 0;
                                for (auto font : Font::GetFamily(text.font_family))
                                {
                                        std::shared_ptr<Character> temp_character = font->GetCharacter(*c);
                                        if (temp_character == nullptr)
                                        {
                                                temp_character = font->RenderCharacter(*c);
                                                if (temp_character != nullptr)
                                                        if (!temp_character->missing)
                                                        {
                                                                character = temp_character;
                                                                break;
                                                        }
                                        }
                                        else if (!temp_character->missing)
                                        {
                                                character = temp_character;
                                                break;
                                        }
                                        std::cout << "i: " << i << std::endl;
                                        i++;
                                }
                                if (character == nullptr)
                                {
                                        //make it the box character
                                        character = Font::GetFamily(text.font_family).at(0)->RenderCharacter(0x0001F533);
                                }

                                auto char_entity = reg.create();
                                reg.emplace<C_Character>(char_entity, C_Character{character});
                                reg.emplace<C_Size>(char_entity, character->Size.x * text.scale, character->Size.y * text.scale);
                                reg.emplace<C_Position>(char_entity);
                                reg.emplace<C_Child>(char_entity, Entity);
                                if (*c != U' ')
                                {
                                        reg.emplace<C_Texture>(char_entity, character->texture);
                                        //default black text
                                        reg.emplace<C_Tint>(char_entity, glm::vec4(0, 0, 0, 1));
                                        //put the text on the same draw layer as it's parent
                                        C_DrawLayer& parent_draw_layer = reg.get<C_DrawLayer>(Entity);
                                        //put the text on the same layer as it's parent and increase it's depth
                                        reg.emplace<C_DrawLayer>(char_entity, parent_draw_layer.layer, parent_draw_layer.depth + 1);
                                }
                                text.character_entities.push_back(char_entity);
                        }
                }
                if (text.recalculate_position_flag)
                {
                        text.recalculate_position_flag = false;
                        glm::vec2 origin = glm::vec2(0.0f, LINE_SPACING * text.scale);
                        text.line_start_character_indicies.clear();

                        std::vector<entt::entity> curr_line;
                        std::vector<entt::entity> curr_word;
                        float word_start_origin;
                        float word_length;
                        int lines = 1;

                        //set the max width of the text to be the width of the object if it has a size component.
                        if (reg.has<C_Size>(Entity))
                                text.max_length = reg.get<C_Size>(Entity).getW();

                        //position all of the characters horizontally, and vertically (relative to eachother)
                        if (text.text.size() > 0)
                                text.line_start_character_indicies.push_back(0);
                        for (int i = 0; i < text.text.size(); i++)
                        {
                                entt::entity char_entity = text.character_entities.at(i);
                                char32_t c = text.text.at(i);
                                Character& character = *reg.get<C_Character>(char_entity).character;
                                //recalculate the size and initial position of the character
                                if (c == ' ')
                                {
                                        reg.get<C_Size>(char_entity).size = glm::vec2((character.Advance >> 6) * text.scale, character.Size.y * text.scale);
                                }
                                else
                                {
                                        reg.get<C_Size>(char_entity).size = glm::vec2(character.Size.x * text.scale, character.Size.y * text.scale);
                                }
                                reg.get<C_Child>(char_entity).offset = glm::vec3(
                                        origin.x + character.Bearing.x * text.scale,
                                        origin.y - character.Bearing.y * text.scale,
                                        0.0f);
                                curr_line.push_back(char_entity);
                                curr_word.push_back(char_entity);
                                origin.x += (character.Advance >> 6) * text.scale;
                                if (c == ' ')
                                {
                                        //restart the word if there's a space
                                        word_start_origin = origin.x;
                                        curr_word.clear();
                                }
                                bool line_overflow = (origin.x + (10 * text.scale)) > text.max_length;
                                if (line_overflow)
                                {
                                        lines++;
                                        //reset the origin and move it down a line
                                        origin.y += LINE_SPACING * text.scale;
                                        if (curr_line.size() <= curr_word.size())
                                        {
                                                word_start_origin = origin.x - (character.Advance >> 6) * text.scale;
                                                curr_word.clear();
                                                curr_word.push_back(char_entity);
                                        }
                                        glm::vec3 line_offset(-word_start_origin, LINE_SPACING * text.scale, 0.0f);

                                        origin.x -= word_start_origin;
                                        //remove the current word from the line
                                        curr_line.erase(curr_line.begin() + (curr_line.size() - curr_word.size()), curr_line.begin() + curr_line.size());

                                        //recalculate the letters of the current word onto the new line
                                        for (entt::entity character_in_word : curr_word)
                                        {
                                                reg.get<C_Child>(character_in_word).offset += line_offset;
                                        }
                                }


                                // format the current line
                                if (line_overflow || i == text.text.size() - 1)
                                {
                                        //calculate the length of the line
                                        //TODO: check if this is a space and if so discard it
                                        entt::entity last_character_entity = curr_line.back();
                                        float line_length = reg.get<C_Child>(last_character_entity).offset.x + (10 * text.scale) + reg.get<C_Size>(last_character_entity).getW();

                                        //set the offset for the line
                                        float line_offset = 0;
                                        switch (text.align)
                                        {
                                        case text_align::LEFT:
                                                line_offset = 0;
                                                break;

                                        case text_align::CENTER:
                                                line_offset = (text.max_length - line_length) * 0.5f;
                                                break;

                                        case text_align::RIGHT:
                                                line_offset = text.max_length - line_length;
                                                break;

                                        case text_align::CENTER_ON_ZERO:
                                                line_offset = -line_length * 0.5f;
                                                break;
                                        }

                                        for (entt::entity character_in_line: curr_line)
                                        {
                                                reg.get<C_Child>(character_in_line).offset.x += line_offset;
                                        }
                                        curr_line.clear();
                                }

                                if (line_overflow)// && i != text.text.size() - 1)
                                {
                                        //add the current word to the new line
                                        for (entt::entity character_in_word : curr_word)
                                        {
                                                curr_line.push_back(character_in_word);
                                        }
                                        word_start_origin = origin.x;
                                        text.line_start_character_indicies.push_back(i - curr_word.size() + 1);
                                        curr_word.clear();
                                }
                        }

                        //calculate the vertical alignment
                        float vertical_text_size = LINE_SPACING * lines * text.scale;
                        float vertical_offset = 0;
                        switch (text.align_vertical)
                        {
                        case text_align_vertical::TOP:
                                vertical_offset = 0;
                                break;

                        case text_align_vertical::BOTTOM:
                                if (reg.has<C_Size>(Entity))
                                        vertical_offset = reg.get<C_Size>(Entity).getH() - vertical_text_size;
                                break;

                        case text_align_vertical::CENTER:
                                if (reg.has<C_Size>(Entity))
                                        vertical_offset = (reg.get<C_Size>(Entity).getH() - vertical_text_size) * 0.5f;
                                break;
                        }
                        text.vertical_offset = vertical_offset;
                        for (entt::entity character_entity : text.character_entities)
                        {
                                reg.get<C_Child>(character_entity).offset.y += vertical_offset;
                                if (reg.has<C_Tint>(character_entity))
                                        if (reg.get<C_Character>(character_entity).character->colored)
                                                reg.get<C_Tint>(character_entity).tint = glm::vec4(1, 1, 1, 1);
                                        else
                                                reg.get<C_Tint>(character_entity).tint = text.color;
                        }
                }
                if (text.can_edit && text.selected)
                {
                        reg.get<C_Size>(text.cursor).size = glm::vec2(std::max(0.05f * LINE_SPACING * text.scale, 3.0f), 0.9f * LINE_SPACING * text.scale);
                        // reg.get<C_Size>(text.cursor).size = glm::vec2(2.0f, 0.85f * LINE_SPACING * text.scale);
                        //get the line the cursor is on
                        C_Child& cursor_child = reg.get<C_Child>(text.cursor);
                        int line = -1;
                        bool at_end = false;
                        if (text.text.size() == 0)
                        {
                                cursor_child.offset.x = 0;
                                line = 0;
                        }
                        else
                        {
                                for (int index : text.line_start_character_indicies)
                                        if (text.cursor_position >= index)
                                                if (text.cursor_position == index && line >= 0)
                                                {
                                                        at_end = true;
                                                        break;
                                                }
                                                else
                                                        line++;
                                        else
                                                break;
                                if (text.cursor_position == text.character_entities.size())
                                {
                                        C_Child& character_child = reg.get<C_Child>(text.character_entities.back());
                                        C_Size& character_size = reg.get<C_Size>(text.character_entities.back());
                                        cursor_child.offset.x = character_child.offset.x + character_size.getW();
                                }
                                else
                                {
                                        if (at_end)
                                        {
                                                C_Child& character_child = reg.get<C_Child>(text.character_entities.at(text.cursor_position - 1));
                                                C_Size& character_size = reg.get<C_Size>(text.character_entities.at(text.cursor_position - 1));
                                                cursor_child.offset.x = character_child.offset.x + character_size.getW();
                                        }
                                        else
                                        {
                                                C_Child& character_child = reg.get<C_Child>(text.character_entities.at(text.cursor_position));
                                                cursor_child.offset.x = floor(character_child.offset.x);
                                        }
                                }
                        }
                        cursor_child.offset.y = (LINE_SPACING * text.scale) * ((float)line + 0.15f) + text.vertical_offset;



                        // reg.get<C_Position>.cursor
                }
                else
                {
                        reg.get<C_Size>(text.cursor).size = glm::vec2(0, 0);
                }
        }
}