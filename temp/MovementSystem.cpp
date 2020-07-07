#include "systems/MovementSystem.hpp"

#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "components/Input_Velocity.hpp"


void MovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
        for (auto entity : es.entities_with_components<Transform, Velocity, Input_Velocity>())
        {
                entityx::ComponentHandle<Transform> transform = entity.component<Transform>();
                entityx::ComponentHandle<Velocity> velocity = entity.component<Velocity>();
                entityx::ComponentHandle<Input_Velocity> input_velocity = entity.component<Input_Velocity>();
                if (transform && velocity && input_velocity)
                {
                        velocity->vel = input_velocity->setVel();
                }
        }


        for (auto entity : es.entities_with_components<Transform, Velocity>())
        {
                entityx::ComponentHandle<Transform> transform = entity.component<Transform>();
                entityx::ComponentHandle<Velocity> velocity = entity.component<Velocity>();
                if (transform && velocity)
                {
                        transform->position += velocity->vel; // * dt ?
                }
        }
}
