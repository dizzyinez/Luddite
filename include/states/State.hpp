#ifndef State_hpp
#define State_hpp

//#include "game.h"
#include "entityx/entityx.h"

class Game;

class State
{
public:
void provideECS(entityx::EventManager* evm,
                entityx::EntityManager* em,
                entityx::SystemManager* sm)
{
        events = evm;
        entities = em;
        systems = sm;
}
entityx::EventManager* events;
entityx::EntityManager* entities;
entityx::SystemManager* systems;
Game* game;

virtual void init() {
};
virtual void handleEvents() {
};
virtual void update(float deltaTime) {
};
virtual void render(float deltaTime) {
};
virtual void clean() {
};
};

#endif
