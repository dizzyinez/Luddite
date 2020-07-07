#ifndef game_hpp
#define game_hpp

#include <iostream>
#include <vector>
#include <stack>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "entityx/entityx.h"

class State;

class Game : public entityx::EntityX {
public:
bool running;

Game();
~Game();
bool Init(GLFWwindow* w);
void HandleEvents();
void Update(float deltaTime);
void Render(float deltaTime);
void Clean();

// states
std::stack<State*> states;

void pushState(State* state);
void popState();
void changeState(State* state);
State* peekState();

GLFWwindow* window;
private:
};

#endif
