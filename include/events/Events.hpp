#ifndef Events_hpp
#define Events_hpp

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <memory>

using EventID = size_t;

class Events;

struct BaseEvent {
public:
        static EventID eventID_counter;
};

template <typename T>
struct Event : public BaseEvent {
private:
        friend class Events;
        static EventID eventID() {
                static EventID eventID = eventID_counter++; //gets a unique ID for each event struct that inherits Event<>
                return eventID;
        }
};

using VecPointer = std::shared_ptr<std::vector<std::shared_ptr<BaseEvent> > >;

class Events
{
public:

template <typename T, typename ... Args>
static void emit(Args && ... args)
{
        get<T>()->push_back(std::make_shared<T>(std::forward<Args>(args) ...));
}

static void flush_all()
{
        auto it = events.begin();
        while (it != events.end())
        {
                it->second->clear(); //clears all of the stored events. should be done after every frame
                it++;
        }
}

template <typename T>
static void flush()
{
        VecPointer t = get<T>(); //clears all of one type of event
        t->clear();
}

template <typename T,typename Func>
static void iterate(Func func)
{
        for (auto it : *get<T>())
                func(std::static_pointer_cast<T>(it));
}
/*
   allows users to easily iterate through a type of event by providing a lambda
   example:
   Events::iterate<TestEvent>([](auto e)
   {

   });
 */



private:

template <typename T>
static VecPointer get()
{
        auto it = events.find(T::eventID());

        if (it == events.end())
        {
                addVector<T>();   //creates the vector if its doesn't exist
                it = events.find(T::eventID());
        }
        return VecPointer(std::static_pointer_cast<std::vector<std::shared_ptr<BaseEvent> > >(it->second));
}

template <typename T>
static void addVector()
{
        VecPointer t = std::make_shared<std::vector<std::shared_ptr<BaseEvent> > >();
        events.emplace(T::eventID(), t);
}

//this is ugly but it works
static std::unordered_map<EventID, VecPointer> events;
};

#endif
