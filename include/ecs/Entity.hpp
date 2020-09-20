#pragma once
#include "entt.hpp"
#include "layers/Layer.hpp"
#include <iostream>
#include "components/NativeScript.hpp"

struct C_NativeScript;
class Entity
{
public:
        Entity() = default;
        Entity(entt::entity id, Layer* layer)
                : m_EntityId(id), m_Layer(layer)
        {
        }

        template <typename T>
        bool HasComponent()
        {
                return m_Layer->m_Registry.has<T>(m_EntityId);
        }
        template <typename T, typename ... Args>
        T& AddComponent(Args&&... args)
        {
                return m_Layer->m_Registry.emplace<T>(m_EntityId, std::forward<Args>(args)...);
        }
        template <typename T>
        T& GetComponent()
        {
                return m_Layer->m_Registry.get<T>(m_EntityId);
        }
        template <typename T>
        void RemoveComponent()
        {
                return m_Layer->m_Registry.remove<T>(m_EntityId);
        }
        template <typename T>
        T& GetContext()
        {
                return m_Layer->m_Registry.ctx<T>();
        }


        template <typename T>
        T* AddScript()
        {
                auto script = AddComponent<C_NativeScript>().Bind<T>(m_EntityId, m_Layer);
                script->OnCreate();
                return script;
                // nsc.m_Script = std::make_unique<T>();
        }

        Entity CreateEntity()
        {
                return m_Layer->CreateEntity();
        }

        entt::entity GetId()
        {
                return m_EntityId;
        }

private:
        entt::entity m_EntityId = entt::null;
        Layer*       m_Layer = nullptr;
};
