#ifndef DrawSystem_hpp
#define DrawSystem_hpp

#include "entityx/entityx.h"
struct DrawSystem : public entityx::System<DrawSystem>
{
public:
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
};

#endif
