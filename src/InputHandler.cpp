#include "InputHandler.hpp"
#include "events/ActionEvent.hpp"
#include "Locator.hpp"
#include "events/WindowResize.hpp"

/*
   TODO:
   send typing events
   send action button events
 */

bool Pressed_Left, Pressed_Right, Pressed_Up, Pressed_Down, H_Recent_Press_Right, V_Recent_Press_Up; //for keyboard movement

std::vector<Input> inputs;

void window_size_callback(GLFWwindow* window, int width, int height)
{
        Locator::getGame()->events.emit<WindowResize>(width, height);
        Locator::getRenderer()->updateMatricies(width, height); //TODO: have the draw system handle this off of the
}

void emplace_motion()
{
        glm::vec2 direction = glm::vec2(
                (Pressed_Left || Pressed_Right) ? (H_Recent_Press_Right ? 1.0f : -1.0f) : 0.0f,
                (Pressed_Up   || Pressed_Down ) ? (V_Recent_Press_Up ? 1.0f : -1.0f) : 0.0f
                );
        if (direction != glm::vec2(0.0f,0.0f))
        {
                direction = glm::normalize(direction);
        }
        inputs.emplace_back(Input(Action::MOTION, Press::NONE, direction));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
        if (action == GLFW_PRESS)
        {
                switch (key) {
                case GLFW_KEY_A:
                        Pressed_Left = true;
                        H_Recent_Press_Right = false;
                        emplace_motion();
                        break;
                case GLFW_KEY_D:
                        Pressed_Right = true;
                        H_Recent_Press_Right = true;
                        emplace_motion();
                        break;
                case GLFW_KEY_W:
                        Pressed_Up = true;
                        V_Recent_Press_Up = true;
                        emplace_motion();
                        break;
                case GLFW_KEY_S:
                        Pressed_Down = true;
                        V_Recent_Press_Up = false;
                        emplace_motion();
                        break;
                }
        }
        else if (action == GLFW_RELEASE)
        {
                switch (key) {
                case GLFW_KEY_A:
                        Pressed_Left = false;
                        H_Recent_Press_Right = true;
                        emplace_motion();
                        break;
                case GLFW_KEY_D:
                        Pressed_Right = false;
                        H_Recent_Press_Right = false;
                        emplace_motion();
                        break;
                case GLFW_KEY_W:
                        Pressed_Up = false;
                        V_Recent_Press_Up = false;
                        emplace_motion();
                        break;
                case GLFW_KEY_S:
                        Pressed_Down = false;
                        V_Recent_Press_Up = true;
                        emplace_motion();
                        break;
                }
        }
}


std::vector<Input>* InputHandler::GetInputs()
{
        return &inputs;
}

void InputHandler::ClearInputs()
{
        inputs.clear();
}

InputHandler::InputHandler()
{

}

void InputHandler::Init()
{
        glfwSetWindowSizeCallback(window, window_size_callback);
        glfwSetKeyCallback(window, key_callback);
}
