#include "layers/L_Game.hpp"
#include <iostream>

#include "ecs/Entity.hpp"

#include "systems/Draw.hpp"
#include "systems/Motion.hpp"
#include "systems/Gui.hpp"
#include "systems/Networking.hpp"
#include "systems/PlayerController.hpp"
#include "systems/Spawning.hpp"
#include "systems/Scripts.hpp"
#include "systems/Animation.hpp"
#include "systems/AnimationBehavior.hpp"
#include "systems/Tileset.hpp"
#include "systems/LocalPlayerInput.hpp"

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
#include "components/Simulation.hpp"
#include "components/Networking.hpp"

#include "events/Events.hpp"
#include "events/Logging.hpp"
#include "events/Input.hpp"
#include "events/Networking.hpp"

#include "script/NativeScript.hpp"
#include "script/PlayerScript.hpp"
#include "data/TextureAllocator.hpp"
#include "data/FontAllocator.hpp"

#include <iostream>

#include "events/Gameworld.hpp"



void L_Game::init()
{
        systems.add<S_Motion>();
        systems.add<S_Gui_Input>();
        systems.add<S_Gui>();
        systems.add<S_Net_Client>();
        systems.add<S_Net_Host>();
        systems.add<S_Net_Send>();
        systems.add<S_Net_Update_Player>();
        systems.add<S_LocalPlayerInput>();
        systems.add<S_PlayerController>();
        systems.add<S_Spawning>();
        systems.add<S_Tileset>();
        systems.add<S_Animation>();
        systems.add<S_AnimationBehavior>();
        systems.add<S_Draw>();
        systems.add<S_Scripts_Events>();
        systems.add<S_Scripts_Update>();
        systems.add<S_Scripts_LateUpdate>();
        systems.configure(m_Registry, this);




        // auto host = CreateEntity();
        // host.AddComponent<C_Position>();
        // host.AddComponent<C_Size>();
        // host.AddComponent<C_DrawLayer>(DrawLayer::gui);
        // host.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        // host.AddComponent<C_Gui>();
        // host.AddComponent<C_Gui_Container>(
        //         [](auto &Gui, auto &Gui_container) {
        //         Gui_container.solver->addEditVariable(Gui.w, kiwi::strength::strong);
        //         Gui_container.solver->addEditVariable(Gui.h, kiwi::strength::strong);
        //         kiwi::Constraint constraints[] = {
        //                 kiwi::Constraint {Gui.x == 0},
        //                 kiwi::Constraint {Gui.y == 0},
        //                 kiwi::Constraint {Gui.h <= 50},
        //                 kiwi::Constraint {Gui.w <= 50}
        //         };
        //         for (auto& constraint : constraints)
        //                 Gui_container.solver->addConstraint(constraint);
        //         Gui_container.solver->updateVariables();
        // },
        //
        //         [](auto &Gui, auto &Gui_container) {
        //         Events::iterateAll<E_WindowResize>([&Gui, &Gui_container](auto e) {
        //                 Gui_container.solver->suggestValue(Gui.w, e->width / 2);
        //                 Gui_container.solver->suggestValue(Gui.h, e->height / 2);
        //                 Gui_container.solver->updateVariables();
        //                 return false;
        //         });
        // });
        // host.AddComponent<C_Gui_Button>([]() {
        //         Events::emit<E_Net_Host>(27015, 32);
        // });


        // auto createPlayer = CreateEntity();
        // createPlayer.AddComponent<C_Position>();
        // createPlayer.AddComponent<C_Size>();
        // createPlayer.AddComponent<C_DrawLayer>(DrawLayer::gui);
        // createPlayer.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        // createPlayer.AddComponent<C_Gui>();
        // createPlayer.AddComponent<C_Gui_Container>(
        //         [](auto &Gui, auto &Gui_container) {
        //         Gui_container.solver->addEditVariable(Gui.w, kiwi::strength::strong);
        //         Gui_container.solver->addEditVariable(Gui.h, kiwi::strength::strong);
        //         kiwi::Constraint constraints[] = {
        //                 kiwi::Constraint {Gui.x >= 110},
        //                 kiwi::Constraint {Gui.y == 0},
        //                 kiwi::Constraint {Gui.h <= 50},
        //                 kiwi::Constraint {Gui.w <= 50}
        //         };
        //         for (auto& constraint : constraints)
        //                 Gui_container.solver->addConstraint(constraint);
        //         Gui_container.solver->updateVariables();
        // },
        //
        //         [](auto &Gui, auto &Gui_container) {
        //         Events::iterateAll<E_WindowResize>([&Gui, &Gui_container](auto e) {
        //                 Gui_container.solver->suggestValue(Gui.w, e->width / 2);
        //                 Gui_container.solver->suggestValue(Gui.h, e->height / 2);
        //                 Gui_container.solver->updateVariables();
        //                 return false;
        //         });
        // });
        // createPlayer.AddComponent<C_Gui_Button>([]() {
        //         Events::emit<E_SpawnPlayer>(1, 0, 0);
        // });
        //
        // auto join = CreateEntity();
        // join.AddComponent<C_Position>();
        // join.AddComponent<C_Size>();
        // join.AddComponent<C_DrawLayer>(DrawLayer::gui);
        // join.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        // join.AddComponent<C_Gui>();
        // join.AddComponent<C_Gui_Container>(
        //         [](auto &Gui, auto &Gui_container) {
        //         Gui_container.solver->addEditVariable(Gui.w, kiwi::strength::strong);
        //         Gui_container.solver->addEditVariable(Gui.h, kiwi::strength::strong);
        //         kiwi::Constraint constraints[] = {
        //                 kiwi::Constraint {Gui.x >= 55},
        //                 kiwi::Constraint {Gui.y == 0},
        //                 kiwi::Constraint {Gui.h <= 50},
        //                 kiwi::Constraint {Gui.w <= 50}
        //         };
        //         for (auto& constraint : constraints)
        //                 Gui_container.solver->addConstraint(constraint);
        //         Gui_container.solver->updateVariables();
        // },
        //
        //         [](auto &Gui, auto &Gui_container) {
        //         Events::iterateAll<E_WindowResize>([&Gui, &Gui_container](auto e) {
        //                 Gui_container.solver->suggestValue(Gui.w, e->width / 2);
        //                 Gui_container.solver->suggestValue(Gui.h, e->height / 2);
        //                 Gui_container.solver->updateVariables();
        //                 return false;
        //         });
        // });
        // join.AddComponent<C_Gui_Button>([]() {
        // Events::emit<E_Net_Connect>("73.14.41.127", 27015);
        // });


        m_Registry.set<C_PlayerSlots>();
        m_Registry.set<C_Net_Client>();
        m_Registry.set<C_Net_Server>();

        m_Registry.set<C_StoredFrames>();



        FontAllocator::AddFakeUser("../assets/fonts/comic.ttf");
        TextureAllocator::AddFakeUser("../assets/textures/wall.jpg");
        auto e1 = CreateEntity();
        e1.AddComponent<C_Position>(-25.0f, 0.0f);
        e1.AddComponent<C_Velocity>(0.0f, 0.0f);
        e1.AddComponent<C_DrawLayer>(DrawLayer::sprite);
        e1.AddComponent<C_Size>(100.0f, 100.0f);
        // e1.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        e1.AddComponent<C_Texture>(FontAllocator::Get("../assets/fonts/comic.ttf")->characters['S'].texture);
        // auto e2 = CreateEntity();
        // e2.AddComponent<C_Position>(25.0f, -50.0f);
        // e2.AddComponent<C_Velocity>(0.0f, 50.0f);
        // e2.AddComponent<C_DrawLayer>();
        // e2.AddComponent<C_Size>(100.0f, 100.0f);
        // e2.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        // auto e3 = CreateEntity();
        // e3.AddComponent<C_Position>(400.0f, 100.0f);
        // e3.AddComponent<C_Velocity>(50.0f, 50.0f);
        // e3.AddComponent<C_DrawLayer>();
        // e3.AddComponent<C_Size>(100.0f, 100.0f);
        // e3.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));


        Events::emit<E_SpawnPlayer>(true);
        // Events::emit<E_SpawnPlayer>(1, 0, 0);
        // Entity test = CreateEntity();
        // test.AddScript<PlayerScript>();
}

void L_Game::handleEvents(float deltaTime)
{
        systems.update<S_Spawning>(deltaTime, m_Registry);
        systems.update<S_Gui_Input>(deltaTime, m_Registry);
        systems.update<S_Net_Client>(deltaTime, m_Registry);
        systems.update<S_Net_Host>(deltaTime, m_Registry);
        systems.update<S_LocalPlayerInput>(deltaTime, m_Registry);
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

void L_Game::update(float deltaTime)
{
        systems.update<S_Gui>(deltaTime, m_Registry);
        systems.update<S_PlayerController>(deltaTime, m_Registry);
        systems.update<S_Animation>(deltaTime, m_Registry);
        systems.update<S_Motion>(deltaTime, m_Registry);
        systems.update<S_Scripts_Update>(deltaTime, m_Registry);



        systems.update<S_Scripts_LateUpdate>(deltaTime, m_Registry);
        systems.update<S_Net_Update_Player>(deltaTime, m_Registry);
        systems.update<S_Net_Send>(deltaTime, m_Registry);

        auto& sf = m_Registry.ctx<C_StoredFrames>();
        entt::registry &reg = sf.frame_array[sf.index];
        // std::cout << "index: " << sf.index << std::endl;

        reg.clear<C_Position, C_Velocity, C_DrawLayer, C_Size, C_Texture, C_PlayerDirection, C_PlayerInput>();

        const auto* to_data = reg.data();
        const auto to_size = reg.size();
        reg.destroy(to_data, to_data + to_size);

        const auto* from_data = m_Registry.data();
        const auto from_size = m_Registry.size();
        reg.assign(from_data, from_data + from_size);

        clone<C_Position>            (m_Registry, reg);
        clone<C_Velocity>            (m_Registry, reg);
        clone<C_DrawLayer>           (m_Registry, reg);
        clone<C_Size>                (m_Registry, reg);
        clone<C_Texture>             (m_Registry, reg);
        clone<C_PlayerDirection>     (m_Registry, reg);
        clone<C_PlayerInput>         (m_Registry, reg);

        sf.index++;
        if (sf.index >= sf.max_frames)
        {
                // // std::cout << "max frames" << std::endl;
                // // auto& sf = m_Registry.ctx<C_StoredFrames>();
                // entt::registry &from = sf.frame_array[0];
                //
                //
                // // const auto* to_data = m_Registry.data();
                // // const auto to_size = m_Registry.size();
                // // m_Registry.destroy(to_data, to_data + to_size);
                // // const auto* from_data = m_Registry.data();
                // // const auto from_size = m_Registry.size();
                // // m_Registry.assign(from_data, from_data + from_size);
                //
                // m_Registry.clear<C_Position, C_Velocity, C_DrawLayer, C_Size, C_Texture, C_PlayerDirection, C_PlayerInput>();
                //
                // clone<C_Position>            (from, m_Registry);
                // clone<C_Velocity>            (from, m_Registry);
                // clone<C_DrawLayer>           (from, m_Registry);
                // clone<C_Size>                (from, m_Registry);
                // clone<C_Texture>             (from, m_Registry);
                // clone<C_PlayerDirection>     (from, m_Registry);
                // clone<C_PlayerInput>         (from, m_Registry);
                // std::cout << "fuck" << std::endl;
                sf.index = 0;
        }

        // using clone_fn_type = void (const entt::registry &, entt::registry &);
        // std::unordered_map<entt::id_type, clone_fn_type *> clone_functions;

        // clone<C_Position>(m_Registry, reg);
        // clone_functions[entt::type_info<C_Position>::id()] = &clone<C_Position>;
        // clone_functions[entt::type_info<C_Velocity>::id()] = &clone<C_Velocity>;
        // clone_functions[entt::type_info<C_Position>::id()](m_Registry, reg);
        // clone_functions[entt::type_info<C_Velocity>::id()](m_Registry, reg);

        // std::cout << "ids:" << std::endl;
        // m_Registry.visit([this, &clone_functions, &reg](const auto type_id) {
        //         std::cout << "id:" << type_id << std::endl;
        //         // clone_functions[type_id](m_Registry, reg);
        // });
        // std::cout << "end of ids:" << std::endl;
}

void L_Game::render(float alpha)
{
        // auto& sf = m_Registry.ctx<C_StoredFrames>();

        systems.update<S_Tileset> (alpha, m_Registry);
        systems.update<S_Draw>    (alpha, m_Registry);
        // systems.update<S_Draw>(deltaTime, sf.frame_array[sf.index]);
}
void L_Game::clean()
{
}

L_Game::L_Game()
{
}
