#ifndef System_hpp
#define System_hpp

#include "entt.hpp"
#include <cstddef>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "entt.hpp"
// #include "layers/Layer.hpp"
// #include "ecs/Entity.hpp"


using SystemID = size_t;

class SystemManager;
class Layer;
class Entity;
class BaseSystem
{
public:

        virtual ~BaseSystem();

        virtual void configure(entt::registry &reg) {
        }

        // virtual void update(float deltaTime, entt::registry &reg) {
        // }

        static SystemID systemID_counter;
        double start_time = 0;
        double end_time = 0;
        Layer* m_Layer;



        Entity CreateEntity();
        Entity ToEntity(entt::entity e);
        // template <typename T>
        // T& GetContext()
        // {
        //         return m_Layer->m_Registry.ctx<T>();
        // }
// template <typename T>
// void view(Func)
// {
//
// }
};

template <typename T>
class System : public BaseSystem
{
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

        template <typename T, typename ... Args>
        void update(Args && ... args)
        {
                std::shared_ptr<T> t = get<T>();
                t->start_time = glfwGetTime();
                t->update(std::forward<Args>(args)...);
                t->end_time = glfwGetTime();
        }

        template <typename T, typename Single, typename ... Ts, typename ... Args>
        void update(Args && ... args)
        {
                update<T>(std::forward<Args>(args)...);
                update<Single, Ts...>(std::forward<Args>(args)...);
        }

        void configure(entt::registry &reg, Layer* layer)
        {
                for (auto s : systems)
                {
                        s.second->m_Layer = layer;
                        s.second->configure(reg);
                }
        }

private:
        template <typename T>
        std::shared_ptr<T> get()
        {
                auto it = systems.find(T::systemID());
                assert(it != systems.end());
                return it == systems.end()
    ? std::shared_ptr<T>()
    : std::shared_ptr<T>(std::static_pointer_cast<T>(it->second));
        }
        std::unordered_map<SystemID, std::shared_ptr<BaseSystem> > systems;
};

#endif
