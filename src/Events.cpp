#include "events/Events.hpp"

EventID BaseEvent::eventID_counter;

std::unordered_map<EventID, VecPointer> Events::events;
