#ifndef System_hpp
#define System_hpp

#include "entt.hpp"
#include <cstddef>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using SystemID = size_t;

class SystemManager;

class BaseSystem {
public:

virtual ~BaseSystem();

virtual void configure(entt::registry &reg) {
}

virtual void update(float deltaTime, entt::registry &reg) {
}

static SystemID systemID_counter;
};

template <typename T>
class System : public BaseSystem {
public:
double run_time()
{
        return end_time - start_time;
}
virtual ~System() {
}
private:
friend class SystemManager;
static SystemID systemID() {
        static SystemID systemID = systemID_counter++;
        return systemID;
}
double start_time = 0;
double end_time = 0;

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
void update(float deltaTime, entt::registry &reg)
{
        std::shared_ptr<T> t = get<T>();
        t->start_time = glfwGetTime();
        t->update(deltaTime, reg);
        t->end_time = glfwGetTime();
}

void configure(entt::registry &reg)
{
        for (auto s : systems)
                s.second->configure(reg);
}

private:
template <typename T>
std::shared_ptr<T> get()
{
        auto it = systems.find(T::systemID());
        assert (it != systems.end());
        return it == systems.end()
    ? std::shared_ptr<T>()
    : std::shared_ptr<T>(std::static_pointer_cast<T>(it->second));
}
std::unordered_map<SystemID, std::shared_ptr<BaseSystem> > systems;
};

#endif
