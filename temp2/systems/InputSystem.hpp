#ifndef InputSystem_hpp
#define InputSystem_hpp

#include "entityx/entityx.h"

struct InputSystem : public entityx::System<InputSystem>
{
public:
        void configure(entityx::EventManager &event_manager);
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
};

#endif
