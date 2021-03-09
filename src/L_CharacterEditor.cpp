#include "layers/L_CharacterEditor.hpp"

#include "ecs/entity.hpp"

#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/Gui.hpp"
#include "components/Text.hpp"
#include "components/Texture.hpp"
#include "components/DrawLayer.hpp"

#include "systems/Draw.hpp"
#include "systems/Ancestry.hpp"
#include "systems/TextRendering.hpp"
#include "systems/Motion.hpp"
#include "systems/Gui.hpp"
#include "systems/Animation.hpp"
#include "systems/Tileset.hpp"


void L_CharacterEditor::init()
{
        systems.add<S_Motion,
                    S_Gui_Input,
                    S_Gui,
                    S_Tileset,
                    S_Animation,
                    S_Ancestry,
                    S_Draw,
                    S_Text_Rendering>();
        systems.configure(m_Registry, this);

        FontAllocator::AddFakeUser("../assets/fonts/ConcertOne-Regular.ttf");

        Entity character_input = CreateEntity();
        character_input.AddComponent<C_Position>();
        character_input.AddComponent<C_Size>();
        character_input.AddComponent<C_DrawLayer>(DrawLayer::gui);
        character_input.AddComponent<C_Tint>(glm::vec4(1, 1, 1, 1));
        character_input.AddComponent<C_Gui>()
        .set_x(PercentConstriant(0.01f, dimensions::WIDTH, edges::LEFT))
        .set_y(PercentConstriant(0.01f, dimensions::WIDTH, edges::TOP))
        .set_w(PercentConstriant(0.2f, dimensions::WIDTH))
        .set_h(PercentConstriant(0.02f, dimensions::WIDTH))
        .set_text_scale(0.7f, dimensions::HEIGHT);
        // character_input.AddComponent<C_Text>(U"😀😂😂😂😂")
        character_input.AddComponent<C_Text>()
        .set_editable(true)
        .set_alignment(text_align::LEFT)
        .set_vertical_alignment(text_align_vertical::TOP);

        Entity load_button = CreateEntity();
        load_button.AddComponent<C_Position>();
        load_button.AddComponent<C_Size>();
        load_button.AddComponent<C_DrawLayer>(DrawLayer::gui);
        load_button.AddComponent<C_Tint>(glm::vec4(1, 1, 1, 1));
        load_button.AddComponent<C_Gui>()
        .set_x(PercentConstriant(0.01f, dimensions::WIDTH, edges::LEFT))
        .set_y(PercentConstriant(0.035f, dimensions::WIDTH, edges::TOP))
        .set_w(PercentConstriant(0.12f, dimensions::WIDTH))
        .set_h(PercentConstriant(0.02f, dimensions::WIDTH))
        .set_text_scale(0.7f, dimensions::HEIGHT);
        load_button.AddComponent<C_Text>(U"Load Character")
        .set_alignment(text_align::CENTER)
        .set_vertical_alignment(text_align_vertical::CENTER);
        load_button.AddComponent<C_Gui_Clickable>()
        .on_click([this, character_input](C_Gui& gui, C_Gui_Clickable& clickable) {
                character_renderer.LoadCharacter(character_input.GetComponent<C_Text>().text);
        });

        Entity export_button = CreateEntity();
        export_button.AddComponent<C_Position>();
        export_button.AddComponent<C_Size>();
        export_button.AddComponent<C_DrawLayer>(DrawLayer::gui);
        export_button.AddComponent<C_Tint>(glm::vec4(1, 1, 1, 1));
        export_button.AddComponent<C_Gui>()
        .set_x(PercentConstriant(0.14f, dimensions::WIDTH, edges::LEFT))
        .set_y(PercentConstriant(0.035f, dimensions::WIDTH, edges::TOP))
        .set_w(PercentConstriant(0.07f, dimensions::WIDTH))
        .set_h(PercentConstriant(0.02f, dimensions::WIDTH))
        .set_text_scale(0.7f, dimensions::HEIGHT);
        export_button.AddComponent<C_Text>(U"Export")
        .set_alignment(text_align::CENTER)
        .set_vertical_alignment(text_align_vertical::CENTER);
        export_button.AddComponent<C_Gui_Clickable>()
        .on_click([this, character_input](C_Gui& gui, C_Gui_Clickable& clickable) {
                character_renderer.ExportCharacter(character_input.GetComponent<C_Text>().text);
        });

        Entity animation_input = CreateEntity();
        animation_input.AddComponent<C_Position>();
        animation_input.AddComponent<C_Size>();
        animation_input.AddComponent<C_DrawLayer>(DrawLayer::gui);
        animation_input.AddComponent<C_Tint>(glm::vec4(1, 1, 1, 1));
        animation_input.AddComponent<C_Gui>()
        .set_x(PercentConstriant(0.01f, dimensions::WIDTH, edges::LEFT))
        .set_y(PercentConstriant(0.08f, dimensions::WIDTH, edges::TOP))
        .set_w(PercentConstriant(0.2f, dimensions::WIDTH))
        .set_h(PercentConstriant(0.02f, dimensions::WIDTH))
        .set_text_scale(0.7f, dimensions::HEIGHT);
        animation_input.AddComponent<C_Text>()
        .set_editable(true)
        .set_alignment(text_align::LEFT)
        .set_vertical_alignment(text_align_vertical::TOP);


        Entity display = CreateEntity();
        // render_button.AddComponent<C_Gui>()
        // .set_x(PercentConstriant(0.21f, dimensions::WIDTH, edges::LEFT))
        // .set_y(PercentConstriant(0.01f, dimensions::HEIGHT, edges::TOP))
        display.AddComponent<C_Position>();
        display.AddComponent<C_Size>();
        display.AddComponent<C_DrawLayer>(DrawLayer::gui);
        display.AddComponent<C_Tint>(glm::vec4(1, 1, 1, 1));
        // display.AddComponent<C_Sprite>(character_renderer.RenderAnimation("ping_pong", "Idle"), glm::uvec4(0xA38F8B, 0x545563, 0xDFBEAB, 0xE77664));
        // display.AddComponent<C_Sprite>(character_renderer.RenderAnimation("ping_pong", "Idle"), glm::uvec4(0xA38F8B, 0x545563, 0xDFBEAB, 0xE77664));



        Entity render_button = CreateEntity();
        render_button.AddComponent<C_Position>();
        render_button.AddComponent<C_Size>();
        render_button.AddComponent<C_DrawLayer>(DrawLayer::gui);
        render_button.AddComponent<C_Tint>(glm::vec4(1, 1, 1, 1));
        render_button.AddComponent<C_Gui>()
        .set_x(PercentConstriant(0.01f, dimensions::WIDTH, edges::LEFT))
        .set_y(PercentConstriant(0.105f, dimensions::WIDTH, edges::TOP))
        .set_w(PercentConstriant(0.2f, dimensions::WIDTH))
        .set_h(PercentConstriant(0.02f, dimensions::WIDTH))
        .set_text_scale(0.7f, dimensions::HEIGHT);
        render_button.AddComponent<C_Text>(U"Render Animation")
        .set_alignment(text_align::CENTER)
        .set_vertical_alignment(text_align_vertical::CENTER);
        render_button.AddComponent<C_Gui_Clickable>()
        .on_click([this, character_input, animation_input, display](C_Gui& gui, C_Gui_Clickable& clickable) {
                if (display.HasComponent<C_Sprite>())
                        display.RemoveComponent<C_Sprite>();
                C_Sprite& sprite = display.AddComponent<C_Sprite>(character_renderer.RenderAnimation(character_input.GetComponent<C_Text>().text, animation_input.GetComponent<C_Text>().text), glm::uvec4(0xA38F8BFF, 0x545563FF, 0xDFBEABFF, 0xE77664FF));
                // C_Sprite& sprite = display.AddComponent<C_Sprite>(character_renderer.RenderAnimation("ping_pong", "Idle"), glm::uvec4(0xA38F8BFF, 0x545563FF, 0xDFBEABFF, 0xE77664FF));
                display.GetComponent<C_Size>().size = glm::vec2(sprite.texture->width, sprite.texture->height);
        });
}

void L_CharacterEditor::handleEvents(float deltaTime)
{
        systems.update<S_Gui_Input>(deltaTime, m_Registry);
}

void L_CharacterEditor::update(float deltaTime)
{
        systems.update<S_Gui>(deltaTime, m_Registry);
        systems.update<S_Animation>(deltaTime, m_Registry);
        systems.update<S_Text_Rendering>(deltaTime, m_Registry);
        systems.update<S_Ancestry>(deltaTime, m_Registry);
        systems.update<S_Motion>(deltaTime, m_Registry);
}

void L_CharacterEditor::render(float alpha, float deltaTime)
{
        systems.update<S_Tileset> (alpha, m_Registry);
        systems.update<S_Draw>    (alpha, m_Registry);
        // character_renderer.RenderAnimation("ping_pong", "Idle");
}

void L_CharacterEditor::clean()
{
}

L_CharacterEditor::L_CharacterEditor()
{
}