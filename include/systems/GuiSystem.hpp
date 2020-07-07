#ifndef GuiSystem_hpp
#define GuiSystem_hpp

#include "entityx/entityx.h"
#include "events/WindowResize.hpp"
struct GuiSystem : public entityx::System<GuiSystem>, public entityx::Receiver<GuiSystem>
{
public:
        void configure(entityx::EventManager &events)
        {
                events.subscribe<WindowResize>(*this);
        }
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
        void receive(const WindowResize &windowResize);
};

#endif
