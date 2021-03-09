#pragma once
#include "ecs/entity.hpp"
#include "layers/Layer.hpp"
#include "data/Assets.hpp"
#include "glm/glm.hpp"



namespace utils
{
Entity SpawnPlayer(Layer *L, uint8_t slot, bool local_player);
Entity SpawnNpc(Layer* L, Npcs::eNpc npc_type, glm::vec2 position = glm::vec2(0.f));
}