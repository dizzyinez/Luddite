#ifndef InputHander_hpp
#define InputHander_hpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "entityx/entityx.h"
#include <glm/glm.hpp>
#include "Input.hpp"

class InputHandler
{

public:
InputHandler();
void provideWindow(GLFWwindow* w)
{
        window = w;
}
void Init();
std::vector<Input>* GetInputs();
void ClearInputs();
private:
GLFWwindow* window;
};

#endif
