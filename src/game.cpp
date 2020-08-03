#include "layers/Layer.hpp"
#include "core/game.hpp"
//#include "TextureAllocator.hpp"
#include "rendering/Renderer.hpp"
#include "Locator.hpp"
#include "core/InputHandling.hpp"

Game::Game() {
}

Game::~Game()
{
        for (Layer* layer : Layers)
                delete layer;
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


void Game::Update(float deltaTime)
{
        std::for_each(std::rbegin(Layers), std::rend(Layers), [deltaTime](auto const& l){
                l->handleEvents(deltaTime);
        });
        // for (auto rit = std::rbegin(Layers); rit != std::rend(Layers); ++rit)
        //         *rit->handleInput(deltaTime);
        for(Layer* layer : Layers)
                layer->update(deltaTime);
}

void Game::Render(float deltaTime)
{
        for(Layer* layer : Layers)
                layer->render(deltaTime);
}

void Game::Clean ()
{
        for(Layer* layer : Layers)
                layer->clean();
}

void Game::PushLayer(Layer* Layer)
{
        Layers.emplace_back(Layer);
        Layer->init();
}

void Game::PopLayer(Layer* layer)
{
        auto it = std::find(Layers.begin(), Layers.end(), layer);
        if (it != Layers.end())
        {
                (*it)->clean();
                Layers.erase(it);
        }
}
