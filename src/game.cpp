#include "layers/Layer.hpp"
#include "core/game.hpp"
//#include "TextureAllocator.hpp"
#include "rendering/Renderer.hpp"
#include "core/InputHandling.hpp"

Game::Game() {
}

Game::~Game()
{
        for (Layer* layer : Layers)
                delete layer;
        //clear events
}

bool Game::Init(GLFWwindow* w)
{
        window = w;
        running = true;

        Renderer::Init();
        Input::Initialize(w);
        return true;
}


void Game::Update(float deltaTime)
{
        std::for_each(std::rbegin(Layers), std::rend(Layers), [deltaTime](Layer* layer) {
                layer->handleEvents(deltaTime);
        });

        for (Layer* layer : Layers)
                layer->update(deltaTime);
        Events::flushAll();
}

void Game::Render(float alpha)
{
        for (Layer* layer : Layers)
                layer->render(alpha);
}

void Game::Clean()
{
        for (Layer* layer : Layers)
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
