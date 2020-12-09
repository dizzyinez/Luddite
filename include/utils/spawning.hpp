#pragma once
#include "ecs/Entity.hpp"
#include "layers/Layer.hpp"



namespace utils
{
Entity SpawnPlayer(Layer *L, uint8_t slot, bool local_player);
}