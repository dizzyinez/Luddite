#pragma once

#include "Layers/Layer.hpp"

class Client;
class Server;

class L_Game : public Layer
{
public:
        void init();
        void handleEvents(float deltaTime);
        void update(float deltaTime);
        void render(float alpha, float deltaTime);
        void clean();
        L_Game();
        std::shared_ptr<Client> client;
        std::shared_ptr<Server> server;
private:
        void Step(float deltaTime, entt::registry& reg);
        void CopyGameState(entt::registry& from, entt::registry& to);
        void CopyInputs(entt::registry& from, entt::registry& to, bool overwrite = true);
        void CopyRenderingComponents(entt::registry& from, entt::registry& to);
        // std::array<Player, 256>* player_list;
};