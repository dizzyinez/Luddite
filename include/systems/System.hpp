#ifndef System_hpp
#define System_hpp

#include "entt.hpp"
#include <cstddef>
#include <unordered_map>
#include <vector>

using SystemID = size_t;

// inline SystemID getNewSystemTypeID()
// {
//         static SystemID lastID = 0;
//         return lastID++;
// }
//
//
// template <typename T> inline SystemID getSystemTypeID()
// {
//         static SystemID typeID = getNewSystemTypeID();
//         return typeID();
// }

class SystemManager;

class BaseSystem {
public:

virtual ~BaseSystem();
/*
   void init(entt::registry reg)  {
        m_Registry = &reg;
   }
 */

virtual void update(float deltaTime, entt::registry &reg) {
}

static SystemID systemID_counter;
};

template <typename T>
class System : public BaseSystem {
public:
virtual ~System() {
}
private:
friend class SystemManager;
static SystemID systemID() {
        static SystemID systemID = systemID_counter++;
        return systemID;
}
};

class SystemManager
{
public:
SystemManager() {
};
template <typename T>
void add(std::shared_ptr<T> system)
{
        systems.emplace(T::systemID(), system);
}

template <typename T, typename ... Args>
std::shared_ptr<T> add(Args && ... args)
{
        std::shared_ptr<T> t(new T(std::forward<Args>(args) ...));
        add(t);
        return t;
}

template <typename T>
std::shared_ptr<T> get()
{
        auto it = systems.find(T::systemID());
        assert (it != systems.end());
        return it == systems.end()
        ? std::shared_ptr<T>()
        : std::shared_ptr<T>(std::static_pointer_cast<T>(it->second));
}

template <typename T>
void update(float deltaTime, entt::registry &reg)
{
        std::shared_ptr<T> t = get<T>();
        t->update(deltaTime, reg);
}

private:
std::unordered_map<SystemID, std::shared_ptr<BaseSystem> > systems;
};

#endif
