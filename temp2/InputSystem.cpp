#include "systems/InputSystem.hpp"
#include <iostream>
#include "Locator.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "events/ActionEvent.hpp"
#include "components/Player.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"

float speed = 300.0f;

void InputSystem::configure(entityx::EventManager &event_manager)
{

}

void InputSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
        std::vector<Input>* inputs = Locator::getInputHandler()->GetInputs();
        for (std::vector<Input>::iterator it = inputs->begin(); it != inputs->end(); ++it)
        {
                if (it->action == Action::MOTION)
                {
                        for (auto entity : es.entities_with_components<Player>())
                        {
                                entityx::ComponentHandle<Player> player = entity.component<Player>();
                                if (player->localPlayer)
                                {
                                        entityx::ComponentHandle<Velocity> velocity = entity.component<Velocity>();
                                        velocity->velocity.x = speed * it->vector.x;
                                        velocity->velocity.y = speed * it->vector.y;
                                }
                        }
                }
        }
        Locator::getInputHandler()->ClearInputs();
}
