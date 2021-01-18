#include "layers/L_CharacterEditor.hpp"

#include "ecs/Entity.hpp"

#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/Gui.hpp"
#include "components/text.hpp"
#include "components/Texture.hpp"
#include "components/DrawLayer.hpp"

#include "systems/Draw.hpp"
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
        .set_x(PercentConstriant(0.1f, dimensions::HEIGHT, edges::LEFT))
        .set_y(PixelConstriant(0.0f, edges::TOP))
        .set_w(PercentConstriant(0.3f, dimensions::HEIGHT))
        .set_h(AspectConstraint(5.0f))
        .set_text_scale(0.9f, dimensions::HEIGHT);
        character_input.AddComponent<C_Text>("Test TEXT")
        .set_editable(true)
        .set_alignment(text_align::LEFT)
        .set_vertical_alignment(text_align_vertical::CENTER);



        // display.AddComponent<C_Sprite>(character_renderer.RenderAnimation("ping_pong", "Idle"), glm::uvec4(0xA38F8B, 0x545563, 0xDFBEAB, 0xE77664));
        // character_renderer.RenderAnimation("ping_pong", "Idle");
        // character_renderer.RenderAnimation("ping_pong", "Run");
        // character_renderer.ExportCharacter("ping_pong");
        // display.AddComponent<C_Tint>(glm::vec4(1, 1, 1, 1));
        // character_renderer.RenderAnimation("ping_pong", "Idle");
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
        systems.update<S_Motion>(deltaTime, m_Registry);
}

void L_CharacterEditor::render(float alpha)
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