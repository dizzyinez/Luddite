#ifndef InputHandling_hpp
#define InputHandling_hpp

#include <GLFW/glfw3.h>
#include "events/Events.hpp"
#include "events/Input.hpp"


struct Input
{
public:
        static void Initialize(GLFWwindow* window)
        {
                glfwSetKeyCallback(window, key_callback);
                glfwSetCursorPosCallback(window, cursor_position_callback);
                glfwSetMouseButtonCallback(window, mouse_button_callback);
        }

private:
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
                //this is kinda memory innificient but since events are cleared every frame,
                //the overhead is tiny, so it's worth the extra ease while programming
                Events::emit<E_KeyAction>(key, scancode, action, mods);
                switch (action) {
                case GLFW_PRESS:
                        Events::emit<E_KeyPress>(key, scancode, mods);
                        Events::emit<E_KeyPressAndRepeat>(key, scancode, mods);
                        break;
                case GLFW_RELEASE:
                        Events::emit<E_KeyRelease>(key, scancode, mods);
                        break;
                case GLFW_REPEAT:
                        Events::emit<E_KeyRepeat>(key, scancode, mods);
                        Events::emit<E_KeyPressAndRepeat>(key, scancode, mods);
                        break;
                }
        }

        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
        {
                Events::emit<E_CusrsorMotion>(xpos, ypos);
        }

        static void window_size_callback(GLFWwindow* window, int width, int height)
        {
                Events::emit<E_WindowResize>(width, height);
        }

        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
                Events::emit<E_MouseButton>(button, action, mods);
                switch (action) {
                case GLFW_PRESS:
                        Events::emit<E_MouseButtonPress>(button, mods);
                        break;
                case GLFW_RELEASE:
                        Events::emit<E_MouseButtonRelease>(button, mods);
                        break;
                }
        }


};
#endif
