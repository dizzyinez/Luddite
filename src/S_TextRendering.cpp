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
                std::shared_ptr<Font> font = FontAllocator::Get("../assets/fonts/ConcertOne-Regular.ttf");
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
                        text.character_entities.reserve(text.text.size());

                        //instantiate new characters
                        for (auto c = text.text.begin(); c != text.text.end(); c++)
                        {
                                if (*c != ' ')
                                {
                                        Character character = font->characters[*c];
                                        auto char_entity = reg.create();
                                        reg.emplace<C_Texture>(char_entity, character.texture);
                                        //default black text
                                        reg.emplace<C_Tint>(char_entity, glm::vec4(0, 0, 0, 1));
                                        //put the text on the same draw layer as it's parent
                                        C_DrawLayer& parent_draw_layer = reg.get<C_DrawLayer>(Entity);
                                        //put the text on the same layer as it's parent and increase it's depth
                                        reg.emplace<C_DrawLayer>(char_entity, parent_draw_layer.layer, parent_draw_layer.depth + 1);
                                        reg.emplace<C_Size>(char_entity, character.Size.x * text.scale, character.Size.y * text.scale);
                                        reg.emplace<C_Position>(char_entity);
                                        reg.emplace<C_Child>(char_entity, Entity);
                                        text.character_entities.push_back(char_entity);
                                }
                                else
                                {
                                        text.character_entities.push_back(entt::null);
                                }
                        }
                }
                if (text.recalculate_position_flag)
                {
                        text.recalculate_position_flag = false;
                        glm::vec2 origin = glm::vec2(0.0f, LINE_SPACING * text.scale);

                        std::vector<entt::entity> curr_line;
                        std::vector<entt::entity> curr_word;
                        float word_start_origin;
                        float word_length;
                        int lines = 1;

                        //set the max width of the text to be the width of the object if it has a size component.
                        if (reg.has<C_Size>(Entity))
                                text.max_length = reg.get<C_Size>(Entity).getW();

                        //position all of the characters horizontally, and vertically (relative to eachother)
                        for (int i = 0; i < text.text.size(); i++)
                        {
                                char c = text.text.at(i);
                                Character& character = font->characters[c];

                                entt::entity char_entity = text.character_entities.at(i);
                                if (char_entity != entt::null)
                                {
                                        //recalculate the size and initial position of the character
                                        reg.get<C_Size>(char_entity).size = glm::vec2(character.Size.x * text.scale, character.Size.y * text.scale);
                                        reg.get<C_Child>(char_entity).offset = glm::vec3(
                                                origin.x + character.Bearing.x * text.scale,
                                                origin.y - character.Bearing.y * text.scale,
                                                0.0f);

                                        curr_line.push_back(char_entity);
                                        curr_word.push_back(char_entity);
                                        origin.x += (character.Advance >> 6) * text.scale;
                                }
                                else
                                {
                                        origin.x += (character.Advance >> 6) * text.scale;
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
                                        glm::vec3 line_offset(-word_start_origin, LINE_SPACING * text.scale, 0.0f);

                                        if (curr_line.size() > curr_word.size())
                                        {
                                                origin.x -= word_start_origin;
                                                //remove the current word from the line
                                                curr_line.erase(curr_line.begin() + (curr_line.size() - curr_word.size()), curr_line.begin() + curr_line.size());

                                                //recalculate the letters of the current word onto the new line
                                                for (entt::entity character_in_word : curr_word)
                                                {
                                                        reg.get<C_Child>(character_in_word).offset += line_offset;
                                                }
                                        }
                                        else
                                        {
                                                origin.x = 0;
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

                        for (entt::entity character_entity : text.character_entities)
                        {
                                if (character_entity != entt::null)
                                        reg.get<C_Child>(character_entity).offset.y += vertical_offset;
                        }
                }
        }
}