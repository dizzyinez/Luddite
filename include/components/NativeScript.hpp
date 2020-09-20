#ifndef C_NativeScript_hpp
#define C_NativeScript_hpp
#include "entt.hpp"
#include "layers/Layer.hpp"
// #include "script/NativeScript.hpp"
// #include "ecs/Entity.hpp"

struct NativeScript;
// class Entity;
struct C_NativeScript
{
        C_NativeScript()
        {
        }
        ~C_NativeScript();
        template <typename T>
        T* Bind(entt::entity e, Layer* layer)
        {
                m_Script = new T();
                BindEntity(e, layer);
                return (T*)m_Script;
        }

        NativeScript* m_Script = nullptr;
private:
        void BindEntity(entt::entity e, Layer* layer);
};

#endif
