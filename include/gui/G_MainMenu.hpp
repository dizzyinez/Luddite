#ifndef G_MainMenu_Hpp
#define G_MainMenu_Hpp

#include "entityx/entityx.h"
#include "events/WindowResize.hpp"
#include "rhea/simplex_solver.hpp"

struct GC_MainMenu : entityx::Component<GC_MainMenu>
{
public:
        rhea::simplex_solver solver;
        GC_MainMenu()
        {
        }
};

struct GS_MainMenu : public entityx::System<GS_MainMenu>, public entityx::Receiver<GS_MainMenu>
{
public:
        void configure(entityx::EventManager &events)
        {
                events.subscribe<WindowResize>(*this);
                events.subscribe<entityx::ComponentAddedEvent<GC_MainMenu> >(*this);
        }
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
        void receive(const WindowResize &windowResize);
        void receive(const entityx::ComponentAddedEvent<GC_MainMenu> &windowResize);
};


#endif
