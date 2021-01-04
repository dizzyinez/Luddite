#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

// #include "CheckGLError.hpp"
#include "core/game.hpp"
#include "events/events.hpp"
#include "events/input.hpp"
#include "events/logging.hpp"
#include "layers/L_Base.hpp"
#include "layers/L_MainMenu.hpp"
#include "layers/L_Game.hpp"
#include <enet/enet.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "data/FontAllocator.hpp"

using namespace glm;

constexpr int UPDATE_RATE = 60;
constexpr float SECONDS_PER_UPDATE = 1.0f / (float)UPDATE_RATE;
// std::unique_ptr<Game> *game = nullptr;
int main(int argc, char *argv[])
{
        glewExperimental = true;
        if (!glfwInit())
        {
                std::cout << "Failed to initialise GLFW" << std::endl;
                return 1;
        }

        glfwWindowHint(GLFW_SAMPLES, 1); // no antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

        GLFWwindow* window = glfwCreateWindow(1024, 768, "VideoGame", NULL, NULL);
        if (!window)
        {
                std::cout << "Failed to open GLFW window." << std::endl;
                glfwTerminate();
                return 1;
        }
        glfwMakeContextCurrent(window); // Initialize GLEW
        glewExperimental = true; // Needed in core profile
        if (glewInit() != GLEW_OK)
        {
                std::cout << "Failed to initialise GLEW" << std::endl;
                return 1;
        }

        Events::emit<E_WindowResize>(1024, 768); //change to whatever is used to initialize the window size int the future

        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL Version supported:  " << glGetString(GL_VERSION) << std::endl;
        int tu = 0;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &tu);
        std::cout << "Maximum Texture Units: " << tu << std::endl;

        if (enet_initialize() != 0)
        {
                fprintf(stderr, "An error occurred while initializing ENet.\n");
                return EXIT_FAILURE;
        }
        else
        {
                std::cout << "enet initialized" << std::endl;
        }
        atexit(enet_deinitialize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        //init
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Dark blue background
        glClearColor(0.0f, 0.6f, 0.6f, 0.0f);
        glfwSwapInterval(0); //v-sync off

        FontAllocator::Initialize();

        // std::unique_ptr<Game> game(new Game()); //why is this a pointer????
        if (Game::Init(window))
        {
                Game::PushLayer(new L_Base());
                Game::PushLayer(new L_MainMenu());

                // double time;
                double accumulator = 0.0;
                double render_accumulator = 0.0;
                double current_time = glfwGetTime();


                double max_fps = 165;
                double min_frame_time = 1.0f / (double)max_fps;
                std::cout << "min frame time: " << min_frame_time << std::endl;
                // double deltaTime;
                // Game::Update(SECONDS_PER_UPDATE);
                while (Game::running && !glfwWindowShouldClose(window))
                {
                        double new_time = glfwGetTime();
                        double frame_time = new_time - current_time;
                        if (frame_time > 0.5)
                        {
                                //TODO: Throw an error or something because the simulation has fallen to much behind
                        }
                        current_time = new_time;
                        accumulator += frame_time;
                        render_accumulator += frame_time;

                        while (accumulator >= SECONDS_PER_UPDATE)
                        {
                                // std::cout << "gamign" << std::endl;
                                // std::cout << deltaTime << " > " << SECONDS_PER_UPDATE << " FPS: " <<int(1.0f / deltaTime) << std::endl;
                                glfwPollEvents();
                                Game::Update(SECONDS_PER_UPDATE);
                                accumulator -= SECONDS_PER_UPDATE;
                        }
                        // std::cout << "render accum: " << render_accumulator << std::endl;
                        if (render_accumulator >= min_frame_time)
                        {
                                // std::cout << "rendering" << std::endl;
                                glClear(GL_COLOR_BUFFER_BIT);
                                Game::Render(accumulator / SECONDS_PER_UPDATE);
                                glfwSwapBuffers(window);

                                render_accumulator -= min_frame_time;
                                while (render_accumulator >= min_frame_time)
                                        render_accumulator -= min_frame_time;
                        }
                }
                Game::Clean();
                std::cout << "game exited" << std::endl;
        }
        else
        {
                std::cout << "Failed to initialize the Game" << std::endl;
        }
        glfwTerminate();
        return 0;
}
