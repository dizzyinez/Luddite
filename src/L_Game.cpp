#include "layers/L_Game.hpp"
#include <iostream>
#include "core/net.hpp"

#include "ecs/Entity.hpp"
#include "utils/cloning.hpp"
#include "utils/spawning.hpp"
#include <glm/gtx/string_cast.hpp>

#include "systems/Draw.hpp"
#include "systems/TextRendering.hpp"
#include "systems/Motion.hpp"
#include "systems/TransformLerp.hpp"
#include "systems/Gui.hpp"
#include "systems/Networking.hpp"
#include "systems/PlayerController.hpp"
#include "systems/Scripts.hpp"
#include "systems/Animation.hpp"
#include "systems/AnimationBehavior.hpp"
#include "systems/Tileset.hpp"
#include "systems/LocalPlayerInput.hpp"

#include "components/GameComponents.hpp"

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
        systems.add<S_Motion,
                    S_Transform_Lerp,
                    S_Gui_Input,
                    S_Gui,
                    S_Net_Client,
                    S_Net_Host,
                    S_Net_Send,
                    S_Net_Update_Player,
                    S_LocalPlayerInput,
                    S_PlayerController,
                    S_Tileset,
                    S_Animation,
                    S_AnimationBehavior,
                    S_Draw,
                    S_Text_Rendering,
                    S_Scripts_Events,
                    S_Scripts_Update,
                    S_Scripts_LateUpdate>();
        systems.configure(m_Registry, this);


        if (server != nullptr)
                SetContext<PlayerList*>(&server->player_list);
        else if (client != nullptr)
                SetContext<PlayerList*>(&client->player_list);
        for (Player& player: *GetContext<PlayerList*>())
        {
                if (player.exists)
                {
                        player.entity = utils::SpawnPlayer(this, player.id, player.local);
                }
        }

        m_Registry.set<C_PlayerSlots>();
        m_Registry.set<C_StoredFrames>();

        // FontAllocator::AddFakeUser("../assets/fonts/comic.ttf");
        // TextureAllocator::AddFakeUser("../assets/textures/wall.jpg");
        // auto e1 = CreateEntity();
        // e1.AddComponent<C_Position>(-25.0f, 0.0f);
        // e1.AddComponent<C_Velocity>(0.0f, 20.0f);
        // e1.AddComponent<C_DrawLayer>(DrawLayer::sprite);
        // e1.AddComponent<C_Size>(100.0f, 100.0f);
        // // e1.AddComponent<C_Texture>(TextureAllocator::Get("../assets/textures/wall.jpg"));
        // e1.AddComponent<C_Texture>(FontAllocator::Get("../assets/fonts/comic.ttf")->characters['S'].texture);


        auto& sf = m_Registry.ctx<C_StoredFrames>();
        CopyGameState(m_Registry, sf.frame_array.at(0));
        for (int i = 1; i < sf.max_frames; i++)
                CopyInputs(m_Registry, sf.frame_array.at(i));
}

void L_Game::handleEvents(float deltaTime)
{
        systems.update<S_Gui_Input>(deltaTime, m_Registry);
        systems.update<S_LocalPlayerInput>(deltaTime, m_Registry);
        //set net validation
        m_Registry.view<C_Player, C_PlayerInput>().each([this](auto& Entity, C_Player& p, C_PlayerInput& pi) {
                if (server != nullptr && p.local_player)
                        pi.net_validated = true;
                else
                        pi.net_validated = false;
        });
        systems.update<S_Scripts_Events>(deltaTime, m_Registry);
}




void L_Game::update(float deltaTime)
{
        CopyRenderingComponents(m_Registry, last_frame);

        C_StoredFrames& sf = m_Registry.ctx<C_StoredFrames>();
        CopyInputs(m_Registry, sf.frame_array.at(sf.index), false);
        // CopyGameState(m_Registry, sf.frame_array.at(0));

        //send player inputs over the network
        //TODO: put this in a system (?)
        m_Registry.view<C_Player>().each([this, &sf](auto Entity, C_Player &player) {
                if (player.local_player)
                {
                        C_PlayerInput& pi = m_Registry.get<C_PlayerInput>(Entity);
                        Message msg;
                        msg.header.id = message_types::PLAYER_INPUT;
                        msg << pi.buttons;
                        msg << sf.current_frame_id();
                        if (client != nullptr)
                        {
                                client->MessageServer(msg);
                        }
                        else if (server != nullptr)
                        {
                                msg << player.player_slot;
                                server->MessageAllClients(msg);
                        }
                }
        });

        //populates server validated inputs (among other things)
        if (server != nullptr)
                server->Update();
        else if (client != nullptr)
                client->Update();


        bool all_inputs_valid = true;
        int index = 0;
        sf.frame_array.at(0).view<C_PlayerInput>().each([&all_inputs_valid](auto Entity, C_PlayerInput &pi) {
                if (!pi.net_validated)
                        all_inputs_valid = false;
        });
        while (all_inputs_valid && index <= sf.index)
        {
                Step(deltaTime, sf.frame_array.at(0));
                index++;
                sf.frame_array.at(index).view<C_PlayerInput>().each([&sf, &all_inputs_valid](auto Entity, C_PlayerInput &pi) {
                        //populate the inputs
                        sf.frame_array.at(0).get<C_PlayerInput>(Entity) = pi;
                        if (!pi.net_validated)
                                all_inputs_valid = false;
                });
        }

        //move the vector back
        if (index > 0)
        {
                for (int i = index + 1; i < sf.max_frames; i++)
                        CopyInputs(sf.frame_array.at(i), sf.frame_array.at(i - index));
        }
        sf.index -= index;
        sf.start_frame_id += index;

        CopyGameState(sf.frame_array.at(0), m_Registry);
        while (index < sf.index)
        {
                Step(deltaTime, m_Registry);
                index++;
                sf.frame_array.at(index).view<C_PlayerInput>().each([this](auto Entity, C_PlayerInput &pi) {
                        //populate the inputs
                        m_Registry.get<C_PlayerInput>(Entity) = pi;
                });
        }
        Step(deltaTime, m_Registry);

        sf.index++;
        CopyRenderingComponents(sf.frame_array.at(0), lerp_frame);
}

void L_Game::Step(float deltaTime, entt::registry& reg)
{
        systems.update<S_Gui,
                       S_PlayerController,
                       S_Animation,
                       S_Motion,
                       S_Scripts_Update,
                       S_Scripts_LateUpdate
                       >(deltaTime, reg);
}

void L_Game::render(float alpha)
{
        systems.update<S_Transform_Lerp> (last_frame, m_Registry, lerp_frame, alpha);
        systems.update<S_Tileset>        (alpha, lerp_frame);
        systems.update<S_Text_Rendering> (alpha, lerp_frame);
        systems.update<S_Draw>           (alpha, lerp_frame);
}


void L_Game::CopyGameState(entt::registry& from, entt::registry& to)
{
        utils::copy_registry<C_Position,
                             C_Velocity,
                             C_Child,
                             C_DrawLayer,
                             C_Size,
                             C_Texture,
                             C_Sprite,
                             C_Tileset,
                             C_Player,
                             C_PlayerInput,
                             C_PlayerDirection,
                             //      C_PlayerKeymap,
                             C_Simulation,
                             C_NativeScript,
                             C_Animation,
                             C_AnimationBehavior,
                             C_AnimationBehaviorState
                             >(from, to);
}
void L_Game::CopyInputs(entt::registry& from, entt::registry& to, bool overwrite)
{
        if (overwrite)
        {
                utils::replace_single_component<C_PlayerInput>(from, to);
        }
        else
        {
                from.view<C_PlayerInput>().each([&to](auto Entity, C_PlayerInput& pi) {
                        if (pi.net_validated)
                                to.get<C_PlayerInput>(Entity) = pi;
                });
        }
}
void L_Game::CopyRenderingComponents(entt::registry& from, entt::registry& to)
{
        utils::clone_registry<C_Position,
                              C_DrawLayer,
                              C_Size,
                              C_Texture,
                              C_Sprite,
                              C_Tileset
                              >(from, to);
}

void L_Game::clean()
{
}

L_Game::L_Game()
{
}
