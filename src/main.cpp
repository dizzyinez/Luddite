#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

// #include "CheckGLError.hpp"

#include "game.hpp"
#include "States/StateMainMenu.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


const int UPDATE_RATE = 500;
const float SECONDS_PER_UPDATE = 1.0f / (float)UPDATE_RATE;
Game *game = nullptr;
int main()
{
        glewExperimental = true;
        if (!glfwInit())
        {
                std::cout << "Failed to initialise GLFW" << std::endl;
                return 1;
        }

        glfwWindowHint(GLFW_SAMPLES, 1); // 0x antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

        GLFWwindow* window = glfwCreateWindow(1024, 768, "VideoGame", NULL, NULL);
        if( !window ) {
                std::cout << "Failed to open GLFW window." << std::endl;
                glfwTerminate();
                return 1;
        }
        glfwMakeContextCurrent(window); // Initialize GLEW
        glewExperimental=true; // Needed in core profile
        if (glewInit() != GLEW_OK) {
                std::cout << "Failed to initialise GLEW" << std::endl;
                return 1;
        }

        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL Version supported:  " << glGetString(GL_VERSION) << std::endl;
        int tu = 0;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &tu);
        std::cout << "Maximum Texture Units: " << tu << std::endl;

        // Dark blue background
        glClearColor(0.0f, 0.3f, 0.6f, 0.0f);
        glfwSwapInterval(0); //v-sync off

        game = new Game();
        if (game->Init(window))
        {
                game->pushState(new StateMainMenu());
                double deltaTime;
                // CheckGLError();
                while (game->running && !glfwWindowShouldClose(window))           // TODO: FIXED UPDATES
                {
                        glClear(GL_COLOR_BUFFER_BIT);

                        deltaTime = glfwGetTime();
                        if ( deltaTime > SECONDS_PER_UPDATE )
                        {
                                // std::cout << deltaTime << " > " << SECONDS_PER_UPDATE << " FPS: " <<int(1.0f / deltaTime) << std::endl;
                                glfwSetTime(0);
                                game->HandleEvents();
                                game->Update(deltaTime);
                                game->Render(deltaTime);
                                glfwSwapBuffers(window);
                        }
                        glfwPollEvents();

                }
                game->Clean();
                std::cout << "game exited" << std::endl;
        }
        else
        {
                std::cout << "Failed to initialise the Game" << std::endl;
        }
        glfwTerminate();
        return 0;
}
