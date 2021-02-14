#include "core/net.hpp"
#include "core/game.hpp"
#include "components/Player.hpp"
#include "components/Simulation.hpp"
#include "layers/L_MainMenu.hpp"
#include "layers/L_Game.hpp"

void Client::OnMessage(Message& msg)
{
        switch (msg.header.id)
        {
        case message_types::CONNECTION_APPROVED:
        {
                std::cout << "connection approved" << std::endl;
                Message out;
                out.header.id = message_types::NEW_PLAYER;
                out << short_string{"Dondomin"};
                MessageServer(out);
        }
        break;

        case message_types::START_GAME:
        {
                std::cout << "Game Started" << std::endl;
                if (l_main_menu != nullptr && l_game == nullptr)
                {
                        l_game = new L_Game();
                        l_game->client = l_main_menu->client; //pass client to game layer
                        Game::PushLayer(l_game);
                        Game::PopLayer(l_main_menu);
                }
                else
                {
                        std::cout << "Server tried to start the game, but the game was already started!" << std::endl;
                }
                // l_main_menu = nullptr;
        }
        break;

        case message_types::NEW_PLAYER:
        {
                uint8_t id;
                short_string name;
                msg >> id >> name;
                Player &player = player_list[id];
                player.id = id;
                player.exists = true;
                player.local = false;
                player.name = name;
        }
        break;

        case message_types::PLAYER_ACK:
        {
                uint8_t id;
                short_string name;
                msg >> id >> name;
                Player &player = player_list[id];
                player.id = id;
                player.exists = true;
                player.local = true;
                player.name = name;
        }
        break;

        case message_types::PLAYER_INPUT:
        {
                uint8_t id;
                msg >> id;
                Player &player = player_list[id];
                // if (player.exists && !player.local)
                if (player.exists)
                {
                        auto& sf = l_game->GetContext<C_StoredFrames>();
                        uint64_t input_frame;
                        msg >> input_frame;
                        int index = sf.frame_id_to_index(input_frame);
                        if (index < 0 || index >= sf.max_frames)
                        {
                                std::cout << "invalid index" << std::endl;
                                break;
                        }

                        if (sf.frame_array.at(index).valid(player.entity.GetId()))
                        {
                                auto& pi = sf.frame_array.at(index).get<C_PlayerInput>(player.entity.GetId());
                                msg >> pi.mouse_direction >> pi.buttons;
                                pi.net_validated = true;
                        }
                }
                else
                        std::cout << "FUCK" << std::endl;
        }
        break;
        }
}