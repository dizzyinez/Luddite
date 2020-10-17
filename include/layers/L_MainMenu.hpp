#ifndef LayerMainMenu_hpp
#define LayerMainMenu_hpp

#include "Layers/Layer.hpp"
#include "core/net.hpp"
#include <memory>

class L_MainMenu : public Layer
{
public:
        void init();
        void handleEvents(float deltaTime);
        void update(float deltaTime);
        void render(float alpha);
        void clean();
        L_MainMenu();
        std::shared_ptr<Client> client;
        std::shared_ptr<Server> server;
};

#endif
