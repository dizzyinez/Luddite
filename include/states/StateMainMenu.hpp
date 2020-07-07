#ifndef StateMainMenu_hpp
#define StateMainMenu_hpp

#include "state.hpp"

class StateMainMenu : public State
{
public:
void init();
void handleEvents();
void update(float deltaTimeq);
void render(float deltaTime);
void clean();
StateMainMenu();//pass through game

private:
//std::shared_ptr<Object> player;
};

#endif
