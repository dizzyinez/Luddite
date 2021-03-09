#include "systems/Projection.hpp"
#include "events/Events.hpp"
#include "events/Input.hpp"
#include "rendering/Renderer.hpp"
#include <iostream>

void S_Projection::update(float deltaTime, entt::registry &reg)
{
        Events::iterateAll<E_WindowResize>([](auto e){
                Renderer::updateMatricies(e->width, e->height);
                return false;
        });
}
