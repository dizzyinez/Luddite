#ifndef MovementSystem_hpp
#define MovementSystem_hpp

#include "entityx/entityx.h"

struct MovementSystem : public entityx::System<MovementSystem>
{
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
};

#endif
