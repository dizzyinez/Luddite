#pragma once

#include "ecs/entity.hpp"
#include "components/Networking.hpp"

struct C_NativeScript;                  // for friend class
// struct Entity;
struct NativeScript
{
public:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnEvents(float deltaTime) {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnLateUpdate(float deltaTime) {}
        // virtual void OnCollision() {}

        template<typename T>
        T &GetComponent()
        {
                return m_Entity.GetComponent<T>();
        }

        template<typename T>
        bool HasComponent()
        {
                return m_Entity.HasComponent<T>();
        }
        template<typename T, typename ... Args>
        T &AddComponent(Args &&... args)
        {
                return m_Entity.AddComponent<T>(std::forward<Args>(args)...);
        }
        template<typename T>
        void RemoveComponent()
        {
                m_Entity.RemoveComponent<T>();
        }
        template<typename T>
        T &GetContext()
        {
                return m_Entity.GetContext<T>();
        }

protected:
        Entity m_Entity;

        friend class C_NativeScript;
        // friend class Layer; ??
};
