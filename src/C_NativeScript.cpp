#include "components/NativeScript.hpp"
#include "script/NativeScript.hpp"
#include "ecs/Entity.hpp"

void C_NativeScript::BindEntity(entt::entity e, Layer* layer)
{
        m_Script->m_Entity = Entity(e, layer);
}

C_NativeScript::~C_NativeScript()
{
        m_Script->OnDestroy();
        delete m_Script;
        //woww
}
