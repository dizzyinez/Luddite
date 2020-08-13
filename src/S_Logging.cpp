#include "systems/Logging.hpp"

#include "events/Events.hpp"
#include "events/Logging.hpp"

#include <iostream>

void S_Logging::update(float deltaTime, entt::registry &reg)
{
        Events::iterate<E_Log>([](auto &e){
                std::cout << *e << std::endl;
                return true;
        });
}
