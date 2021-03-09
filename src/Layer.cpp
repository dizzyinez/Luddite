#include "layers/Layer.hpp"
#include "ecs/entity.hpp"

Entity Layer::CreateEntity()
{
        return {m_Registry.create(), this};
}

Entity Layer::ToEntity(entt::entity e)
{
        return {e, this};
}
