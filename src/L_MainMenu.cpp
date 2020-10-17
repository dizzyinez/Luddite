#include "layers/L_MainMenu.hpp"
#include "config.h"
#include <iostream>

#include "ecs/Entity.hpp"

#include "systems/Draw.hpp"
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

#include "events/Events.hpp"
#include "events/Logging.hpp"
#include "events/Input.hpp"
#include "events/Networking.hpp"

#include "script/NativeScript.hpp"
#include "script/PlayerScript.hpp"
#include "data/TextureAllocator.hpp"
#include "data/FontAllocator.hpp"

#include <iostream>


void L_MainMenu::init()
{
        systems.add<S_Motion>();
        systems.add<S_Gui_Input>();
        systems.add<S_Gui>();
        systems.add<S_Tileset>();
        systems.add<S_Animation>();
        systems.add<S_Draw>();
        systems.add<S_Scripts_Events>();
        systems.add<S_Scripts_Update>();
        systems.add<S_Scripts_LateUpdate>();
        systems.configure(m_Registry, this);




        auto join = CreateEntity();
        join.AddComponent<C_Position>();
        join.AddComponent<C_Size>();
        join.AddComponent<C_DrawLayer>(DrawLayer::gui);
        join.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        join.AddComponent<C_Gui>();
        join.AddComponent<C_Gui_Container>(
                [](auto &Gui, auto &Gui_container) {
                Gui_container.solver->addEditVariable(Gui.w, kiwi::strength::strong);
                Gui_container.solver->addEditVariable(Gui.h, kiwi::strength::strong);
                kiwi::Constraint constraints[] = {
                        kiwi::Constraint {Gui.x == 55},
                        kiwi::Constraint {Gui.y == 0},
                        kiwi::Constraint {Gui.h <= 50},
                        kiwi::Constraint {Gui.w <= 50}
                };
                for (auto& constraint : constraints)
                        Gui_container.solver->addConstraint(constraint);
                Gui_container.solver->updateVariables();
        },

                [](auto &Gui, auto &Gui_container) {
                Events::iterateAll<E_WindowResize>([&Gui, &Gui_container](auto e) {
                        Gui_container.solver->suggestValue(Gui.w, e->width / 2);
                        Gui_container.solver->suggestValue(Gui.h, e->height / 2);
                        Gui_container.solver->updateVariables();
                        return false;
                });
        });
        join.AddComponent<C_Gui_Button>([this]() {
                client = std::make_shared<Client>();
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


        auto host = CreateEntity();
        host.AddComponent<C_Position>();
        host.AddComponent<C_Size>();
        host.AddComponent<C_DrawLayer>(DrawLayer::gui);
        host.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        host.AddComponent<C_Gui>();
        host.AddComponent<C_Gui_Container>(
                [](auto &Gui, auto &Gui_container) {
                Gui_container.solver->addEditVariable(Gui.w, kiwi::strength::strong);
                Gui_container.solver->addEditVariable(Gui.h, kiwi::strength::strong);
                kiwi::Constraint constraints[] = {
                        kiwi::Constraint {Gui.x == 0},
                        kiwi::Constraint {Gui.y == 0},
                        kiwi::Constraint {Gui.h <= 50},
                        kiwi::Constraint {Gui.w <= 50}
                };
                for (auto& constraint : constraints)
                        Gui_container.solver->addConstraint(constraint);
                Gui_container.solver->updateVariables();
        },

                [](auto &Gui, auto &Gui_container) {
                Events::iterateAll<E_WindowResize>([&Gui, &Gui_container](auto e) {
                        Gui_container.solver->suggestValue(Gui.w, e->width / 2);
                        Gui_container.solver->suggestValue(Gui.h, e->height / 2);
                        Gui_container.solver->updateVariables();
                        return false;
                });
        });
        host.AddComponent<C_Gui_Button>([this]() {
                server = std::make_shared<Server>();
                server->Start();
                // Events::emit<E_Net_Host>(1234, 2);
        });

        // FontAllocator::AddFakeUser("../assets/fonts/comic.ttf");
        TextureAllocator::AddFakeUser("../assets/textures/wall.jpg");
        auto e1 = CreateEntity();
        e1.AddComponent<C_Position>(-25.0f, 0.0f);
        e1.AddComponent<C_Velocity>(0.0f, 0.0f);
        e1.AddComponent<C_DrawLayer>(DrawLayer::sprite);
        e1.AddComponent<C_Size>(100.0f, 100.0f);
        // e1.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        e1.AddComponent<C_Texture>(FontAllocator::Get("../assets/fonts/comic.ttf")->characters['S'].texture);
}

void L_MainMenu::handleEvents(float deltaTime)
{
        systems.update<S_Gui_Input>(deltaTime, m_Registry);
        systems.update<S_Scripts_Events>(deltaTime, m_Registry);
}


template<typename T>
void clone(const entt::registry &from, entt::registry &to) {
        const auto *data = from.data<T>();
        const auto size = from.size<T>();

        if constexpr (ENTT_IS_EMPTY(T)) {
                to.insert<T>(data, data + size);
        }
        else
        {
                const auto *raw = from.raw<T>();
                // to.assign(data, data + size);
                to.insert<T>(data, data + size, raw, raw + size);
        }
}

void L_MainMenu::update(float deltaTime)
{
        systems.update<S_Gui>(deltaTime, m_Registry);
        systems.update<S_Animation>(deltaTime, m_Registry);
        systems.update<S_Motion>(deltaTime, m_Registry);
        systems.update<S_Scripts_Update>(deltaTime, m_Registry);
        if (server != nullptr)
                server->Update();
        else if (client != nullptr)
                client->Update();


        systems.update<S_Scripts_LateUpdate>(deltaTime, m_Registry);
}

void L_MainMenu::render(float alpha)
{
        // auto& sf = m_Registry.ctx<C_StoredFrames>();

        systems.update<S_Tileset> (alpha, m_Registry);
        systems.update<S_Draw>    (alpha, m_Registry);
        // systems.update<S_Draw>(deltaTime, sf.frame_array[sf.index]);
}
void L_MainMenu::clean()
{
}

L_MainMenu::L_MainMenu()
{
}
