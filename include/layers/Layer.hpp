#ifndef Layer_hpp
#define Layer_hpp

//#include "game.h"
#include <stack>
#include "entt.hpp"
#include "systems/System.hpp"

class Game;
class Entity;

class Layer
{
public:
Game* game;


virtual void init() {
}
virtual void handleEvents(float deltaTime) {
}
virtual void update(float deltaTime) {
}
virtual void render(float deltaTime) {
}
virtual void clean() {
}

Entity CreateEntity();
Entity ToEntity(entt::entity);

protected:
entt::registry m_Registry;
SystemManager systems;
friend class Entity;
};

#endif
