#include "systems/Scripts.hpp"
#include "components/NativeScript.hpp"
#include "script/NativeScript.hpp"

void S_Scripts_Events::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_NativeScript>().each([deltaTime](auto entity, auto &nsc) {
                if (nsc.m_Script)
                        nsc.m_Script->OnEvents(deltaTime);
        });
}

void S_Scripts_Update::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_NativeScript>().each([deltaTime, this](auto entity, auto &nsc) {
                if (nsc.m_Script == nullptr)
                {
                        nsc.m_Script = nsc.InstantiateScript();
                        nsc.m_Script->m_Entity = ToEntity(entity);
                        nsc.m_Script->OnCreate();
                }
                nsc.m_Script->OnUpdate(deltaTime);
        });
}

void S_Scripts_LateUpdate::update(float deltaTime, entt::registry &reg)
{
        reg.view<C_NativeScript>().each([deltaTime](auto entity, auto &nsc) {
                if (nsc.m_Script != nullptr)
                        nsc.m_Script->OnLateUpdate(deltaTime);
        });
}
