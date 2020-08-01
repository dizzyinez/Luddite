#include "states/State.hpp"
#include "core/game.hpp"
//#include "TextureAllocator.hpp"
#include "rendering/Renderer.hpp"
#include "Locator.hpp"
#include "core/InputHandling.hpp"

Game::Game() {
}

Game::~Game()
{
        // SDL_DestroyRenderer(renderer);
        // SDL_DestroyWindow(window);
        //TTF_Quit();
        //Mix_Quit();
        // IMG_Quit();
        // SDL_Quit();
        //clear events
}

bool Game::Init(GLFWwindow* w)
{
        window = w;
        running = true;

        Renderer::Init();
        // Locator::provideRenderer(renderer);
        Locator::provideWindow(window);
        Locator::Initialize();
        // Locator::getTexureAllocator()->setWindowIcon("../assets/textures/test.png");
        Locator::provideGame(this);
        Input::Initialize(w);
        return true;
}

void Game::HandleEvents()
{
        /*
              events.clear();
              while(SDL_PollEvent(&event))
              {
                      events.push_back(event);
                      switch (event.type)
                      {
                      case SDL_QUIT:
                              running = false;
                              break;
                      default:
                              if (peekState() != nullptr)
                              {
                                      peekState()->handleEvents(event); //pass through events vector
                              }
                      }
              }*/
}

void Game::Update(float deltaTime)
{
        if (peekState() != nullptr)
        {
                peekState()->update(deltaTime);
        }
}

void Game::Render(float deltaTime)
{
        // glClear( GL_COLOR_BUFFER_BIT );
        // SDL_RenderClear(renderer);
        if (peekState() != nullptr)
        {
                peekState()->render(deltaTime);
        }
        // glfwSwapBuffers(window);
        // glfwPollEvents();
        //render the current state
        // SDL_RenderPresent(renderer);
}

void Game::Clean ()
{

}

void Game::pushState(State* state)
{
        states.push(state);
        state->init();
}

void Game::popState()
{
        peekState()->clean();
        delete states.top();
        states.pop();
}

void Game::changeState(State* state)
{
        if (!states.empty()) popState();
        pushState(state);
}

State* Game::peekState()
{
        if (states.empty()) return nullptr;

        return states.top();
}
