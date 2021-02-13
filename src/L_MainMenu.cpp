#include "layers/L_MainMenu.hpp"
#include "config.h"
#include <iostream>
#include "core/net.hpp"
#include "core/game.hpp"

#include "ecs/Entity.hpp"

#include "systems/Draw.hpp"
#include "systems/TextRendering.hpp"
#include "systems/Motion.hpp"
#include "systems/Gui.hpp"
#include "systems/Scripts.hpp"
#include "systems/Animation.hpp"
#include "systems/Tileset.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Drag.hpp"
#include "components/Size.hpp"
#include "components/DrawLayer.hpp"
#include "components/Gui.hpp"
#include "components/Player.hpp"
#include "components/PlayerKeymap.hpp"
#include "components/NativeScript.hpp"
#include "components/Texture.hpp"
#include "components/text.hpp"

#include "events/Events.hpp"
#include "events/Logging.hpp"
#include "events/Input.hpp"
#include "events/Networking.hpp"

#include "script/NativeScript.hpp"
#include "script/PlayerScript.hpp"
#include "data/TextureAllocator.hpp"
#include "data/FontAllocator.hpp"

#include "layers/L_Game.hpp"
#include "layers/L_CharacterEditor.hpp"

#include <iostream>

void L_MainMenu::init()
{
        systems.add<S_Motion,
                    S_Gui_Input,
                    S_Gui,
                    S_Tileset,
                    S_Animation,
                    S_Draw,
                    S_Text_Rendering,
                    S_Scripts_Events,
                    S_Scripts_Update,
                    S_Scripts_LateUpdate>();
        systems.configure(m_Registry, this);

        FontAllocator::AddFakeUser("../assets/fonts/ConcertOne-Regular.ttf");

        Entity join_button = CreateEntity();
        join_button.AddComponent<C_Position>();
        join_button.AddComponent<C_Size>();
        join_button.AddComponent<C_DrawLayer>(DrawLayer::gui);
        // join_button.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        join_button.AddComponent<C_Tint>(glm::vec4(1, 1, 0.5f, 1));
        join_button.AddComponent<C_Gui>()
        .set_text_scale(0.35, dimensions::HEIGHT)
        .set_x(PercentConstriant(-0.1, dimensions::HEIGHT, edges::LEFT))
        .set_y(PercentConstriant(0.4, dimensions::HEIGHT))
        .set_w(PercentConstriant(0.3f, dimensions::HEIGHT))
        .set_h(AspectConstraint(3.0f));
        join_button.AddComponent<C_Gui_Clickable>()
        .on_hover_over([](C_Gui& Gui, C_Gui_Clickable& Clickable) {Gui.transition(Transition(0.1f).set_x_offset(0.1f, dimensions::HEIGHT));})
        .reset_on_hover_away(0.1f)
        .on_click([this](C_Gui& Gui, C_Gui_Clickable& Clickable) {
                client = std::make_shared<Client>();
                client->l_main_menu = (L_MainMenu*)this;
                if (client->Connect())
                {
                        Message msg;
                        msg.header.id = message_types::CONNECTION_REQUEST;
                        int version_major = VERSION_MAJOR;
                        int version_minor = VERSION_MINOR;
                        msg << version_major << version_minor;
                        client->MessageServer(msg);
                        std::cout << "size of body: " << msg.header.body_size << std::endl;
                        client->flush();
                }
                // Events::emit<E_Net_Host>(1234, 2);
        });
        join_button.AddComponent<C_Text>(U"Join Game")
        .set_alignment(text_align::RIGHT)
        .set_vertical_alignment(text_align_vertical::CENTER);

        auto start = CreateEntity();
        start.AddComponent<C_Position>();
        start.AddComponent<C_Size>();
        start.AddComponent<C_DrawLayer>(DrawLayer::gui);
        start.AddComponent<C_Tint>(glm::vec4(1, 1, 0.5f, 0.5f));
        start.AddComponent<C_Gui>()
        .set_text_scale(0.35, dimensions::HEIGHT)
        .set_x(PercentConstriant(-0.1, dimensions::HEIGHT, edges::LEFT))
        .set_y(PercentConstriant(0.62, dimensions::HEIGHT))
        .set_w(PercentConstriant(0.3f, dimensions::HEIGHT))
        .set_h(AspectConstraint(3.0f));
        start.AddComponent<C_Text>(U"Start Game")
        .set_alignment(text_align::RIGHT)
        .set_alpha(0.5)
        .set_vertical_alignment(text_align_vertical::CENTER);

        auto host = CreateEntity();
        host.AddComponent<C_Position>();
        host.AddComponent<C_Size>();
        host.AddComponent<C_DrawLayer>(DrawLayer::gui);
        host.AddComponent<C_Tint>(glm::vec4(1, 1, 0.5f, 1));
        host.AddComponent<C_Gui>()
        .set_text_scale(0.35, dimensions::HEIGHT)
        .set_x(PercentConstriant(-0.1, dimensions::HEIGHT, edges::LEFT))
        .set_y(PercentConstriant(0.51, dimensions::HEIGHT))
        .set_w(PercentConstriant(0.3f, dimensions::HEIGHT))
        .set_h(AspectConstraint(3.0f));
        host.AddComponent<C_Gui_Clickable>()
        .on_hover_over([](C_Gui& Gui, C_Gui_Clickable& Clickable) {Gui.transition(Transition(0.1f).set_x_offset(0.1f, dimensions::HEIGHT));})
        .reset_on_hover_away(0.1f)
        .on_click([this, start](C_Gui& Gui, C_Gui_Clickable& Clickable) {
                server = std::make_shared<Server>();
                server->Start();
                server->l_main_menu = (L_MainMenu*)this;
                server->AddLocalPlayer(short_string{"Game host"});
                // Events::emit<E_Net_Host>(1234, 2);

                start.GetComponent<C_Tint>().tint.a = 1;
                start.GetComponent<C_Text>().set_alpha(1);
                start.AddComponent<C_Gui_Clickable>()
                .on_hover_over([](C_Gui& Gui, C_Gui_Clickable& Clickable) {Gui.transition(Transition(0.1f).set_x_offset(0.1f, dimensions::HEIGHT));})
                .reset_on_hover_away(0.1f)
                .on_click([this](C_Gui& Gui, C_Gui_Clickable& Clickable) {
                        if (server != nullptr)
                        {
                                Message msg;
                                msg.header.id = message_types::START_GAME;
                                server->MessageAllClients(msg);

                                server->l_game = new L_Game();
                                server->l_game->server = server; //pass server to game layer
                                Game::PushLayer(server->l_game);
                                Game::PopLayer(server->l_main_menu);
                                // Events::emit<E_Net_Host>(1234, 2);
                        }
                });
        });
        host.AddComponent<C_Text>(U"Host Game")
        .set_alignment(text_align::RIGHT)
        .set_vertical_alignment(text_align_vertical::CENTER);

        auto editor = CreateEntity();
        editor.AddComponent<C_Position>();
        editor.AddComponent<C_Size>();
        editor.AddComponent<C_DrawLayer>(DrawLayer::gui);
        editor.AddComponent<C_Tint>(glm::vec4(1, 1, 0.5f, 1));
        editor.AddComponent<C_Gui>()
        .set_text_scale(0.35, dimensions::HEIGHT)
        .set_x(PercentConstriant(-0.1, dimensions::HEIGHT, edges::LEFT))
        .set_y(PercentConstriant(0.73, dimensions::HEIGHT))
        .set_w(PercentConstriant(0.3f, dimensions::HEIGHT))
        .set_h(AspectConstraint(3.0f));
        editor.AddComponent<C_Text>(U"Character Editor")
        .set_alignment(text_align::RIGHT)
        .set_vertical_alignment(text_align_vertical::CENTER);
        editor.AddComponent<C_Gui_Clickable>()
        .on_hover_over([](C_Gui& Gui, C_Gui_Clickable& Clickable) {Gui.transition(Transition(0.1f).set_x_offset(0.1f, dimensions::HEIGHT));})
        .reset_on_hover_away(0.1f)
        .on_click([this](C_Gui& Gui, C_Gui_Clickable& Clickable) {
                Game::PushLayer(new L_CharacterEditor());
                Game::PopLayer(this);
        });
}

void L_MainMenu::handleEvents(float deltaTime)
{
        systems.update<S_Gui_Input>(deltaTime, m_Registry);
        systems.update<S_Scripts_Events>(deltaTime, m_Registry);
}

void L_MainMenu::update(float deltaTime)
{
        systems.update<S_Gui>(deltaTime, m_Registry);
        systems.update<S_Animation>(deltaTime, m_Registry);
        systems.update<S_Text_Rendering>(deltaTime, m_Registry);
        systems.update<S_Motion>(deltaTime, m_Registry);
        systems.update<S_Scripts_Update>(deltaTime, m_Registry);


        systems.update<S_Scripts_LateUpdate>(deltaTime, m_Registry);
        if (server != nullptr)
                server->Update();
        else if (client != nullptr)
                client->Update();
}

void L_MainMenu::render(float alpha)
{
        systems.update<S_Tileset> (alpha, m_Registry);
        systems.update<S_Draw>    (alpha, m_Registry);
}
void L_MainMenu::clean()
{
}

L_MainMenu::L_MainMenu()
{
}
