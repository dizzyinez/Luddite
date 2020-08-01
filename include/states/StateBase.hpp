#ifndef StateBase_hpp
#define StateBase_hpp

#include "states/state.hpp"

class StateBase : public State
{
public:
void init();
void handleEvents();
void update(float deltaTime);
void render(float deltaTime);
void clean();
StateBase();
};

#endif
