#include "components/NativeScript.hpp"
#include "script/NativeScript.hpp"
#include "ecs/Entity.hpp"

void C_NativeScript::BindEntity(entt::entity e, Layer* layer)
{
        m_Script->m_Entity = Entity(e, layer);
}

C_NativeScript::~C_NativeScript()
{
        // std::cout << "native script destructor called" << std::endl;
        // if (m_Script)
        // {
        //         m_Script->OnDestroy();
        //         DestroyScript(this);
        // }
        // delete m_Script;
        // m_Script = nullptr;
        //woww
}
