#include "layers/Layer.hpp"
#include "core/game.hpp"
//#include "TextureAllocator.hpp"
#include "rendering/Renderer.hpp"
#include "core/InputHandling.hpp"

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
        update_queue();
        std::for_each(std::rbegin(Layers), std::rend(Layers), [deltaTime](Layer *layer) {
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

void Game::PushLayer(Layer* layer)
{
        add_queue.emplace_back(layer);
}

void Game::PopLayer(Layer* layer)
{
        remove_queue.emplace_back(layer);
}

void Game::update_queue()
{
        for (Layer* layer : remove_queue)
                pop_layer(layer);
        for (Layer* layer : add_queue)
                push_layer(layer);
        remove_queue.clear();
        add_queue.clear();
}

void Game::push_layer(Layer* layer)
{
        Layers.emplace_back(layer);
        layer->init();
}

void Game::pop_layer(Layer* layer)
{
        auto it = std::find(Layers.begin(), Layers.end(), layer);
        if (it != Layers.end())
        {
                (*it)->clean();
                delete *it;
                Layers.erase(it);
        }
}
