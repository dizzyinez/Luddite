#ifndef E_Logging_hpp
#define E_Logging_hpp

#include "events/Events.hpp"
#include <string>
#include <iostream>

struct E_Log : public Event<E_Log>
{
        E_Log(std::string message_)
                : message(message_)
        {

        }
        E_Log(char* message_)
                : message(message_)
        {

        }
        std::string message;
        friend std::ostream &operator << (std::ostream &output, E_Log &e) {
                output << e.message;
                return output;
        }
};

#endif
