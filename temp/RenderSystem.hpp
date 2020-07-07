#ifndef RenderSystem
#define RenderSystem

#include "entityx/entityx.h"

#include "events/RenderEvent"

struct RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem>
{
        void configure(entityx::EventManager &event_manager)
        {
                event_manager.subscribe<RenderEvent>(*this)
        }
};

//if valid()

#endif
