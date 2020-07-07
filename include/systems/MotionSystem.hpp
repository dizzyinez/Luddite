#ifndef MotionSystem_hpp
#define MotionSystem_hpp

#include "entityx/entityx.h"

struct MotionSystem : public entityx::System<MotionSystem>
{
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
};

#endif
