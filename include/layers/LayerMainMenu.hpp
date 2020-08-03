#ifndef LayerMainMenu_hpp
#define LayerMainMenu_hpp

#include "Layers/Layer.hpp"

class L_MainMenu : public Layer
{
public:
void init();
void handleEvents(float deltaTime);
void update(float deltaTime);
void render(float deltaTime);
void clean();
L_MainMenu();
};

#endif
