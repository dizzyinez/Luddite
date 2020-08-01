#ifndef StateMainMenu_hpp
#define StateMainMenu_hpp

#include "states/state.hpp"

class StateMainMenu : public State
{
public:
void init();
void handleEvents();
void update(float deltaTime);
void render(float deltaTime);
void clean();
StateMainMenu();
};

#endif
