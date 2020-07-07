#include "systems/MotionSystem.hpp"

#include "components/Transform.hpp"
#include "components/Velocity.hpp"


void MotionSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
        for (auto entity : es.entities_with_components<Velocity, Transform>())
        {
                entityx::ComponentHandle<Transform> transform = entity.component<Transform>();
                entityx::ComponentHandle<Velocity> velocity = entity.component<Velocity>();
                if (transform && velocity)
                {
                        transform->position += velocity->velocity * (float)dt;
                }
        }
}
