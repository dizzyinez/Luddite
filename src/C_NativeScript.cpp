#include "components/NativeScript.hpp"
#include "script/NativeScript.hpp"
#include "ecs/entity.hpp"

void C_NativeScript::BindEntity(entt::entity e, Layer* layer)
{
        m_Script->m_Entity = Entity(e, layer);
}