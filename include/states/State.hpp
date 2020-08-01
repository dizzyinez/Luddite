#ifndef State_hpp
#define State_hpp

//#include "game.h"
#include <stack>
#include "entt.hpp"
#include "systems/System.hpp"

class Game;

class State
{
public:
Game* game;


virtual void init() {
}
virtual void handleEvents() {
}
virtual void update(float deltaTime) {
}
virtual void render(float deltaTime) {
}
virtual void clean() {
}

entt::entity createEntity()
{
        return m_Registry.create();
}

protected:
entt::registry m_Registry;
SystemManager systems;


// template <typename T>
// void addSystem()
// {
//
// }
};

#endif
