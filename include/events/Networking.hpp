#ifndef E_Networking
#define E_Networking

#include "events/Events.hpp"


struct E_Connect : public Event<E_Connect>
{
        E_Connect(const char * hostName_)
                : hostName(hostName_)
        {
        }
        const char * hostName;
};

#endif
