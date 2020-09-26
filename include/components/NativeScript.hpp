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


        NativeScript*(*InstantiateScript)();
        void (*DestroyScript)(C_NativeScript*);
        template <typename T>
        void Bind(entt::entity e, Layer* layer)
        {
                InstantiateScript = []() {return static_cast<NativeScript*>(new T());};
                DestroyScript = [](C_NativeScript* nsc) {delete nsc->m_Script; nsc->m_Script = nullptr;};
        }
        //         m_Script = new T();
        //         BindEntity(e, layer);
        //         return (T*)m_Script;

        NativeScript* m_Script = nullptr;
private:
        void BindEntity(entt::entity e, Layer* layer);
        std::function<void()> DestroyInstanceFunction;
};

#endif
