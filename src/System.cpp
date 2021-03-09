#include "systems/System.hpp"
#include "ecs/entity.hpp"
#include "layers/Layer.hpp"

SystemID BaseSystem::systemID_counter;

BaseSystem::~BaseSystem() {}
Entity BaseSystem::CreateEntity()
{
        return m_Layer->CreateEntity();
}

Entity BaseSystem::ToEntity(entt::entity e)
{
        return m_Layer->ToEntity(e);
}
