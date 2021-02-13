#include "systems/Hitboxes.hpp"
#include "components/GameComponents.hpp"
#include <iostream>


void S_Hitboxes::update(float deltaTime, entt::registry &reg)
{
        for (auto &&[entity, hitbox, collider, origin, pos]: reg.group<C_Hitbox>(entt::get<C_CircleCollider, C_Origin, C_Position>).each())
        {
                glm::vec2 hitbox_center = glm::vec2(pos.position) + origin.origin;
                for (auto &&[p_entity, p_player, p_collider, p_origin, p_pos]: reg.group<C_Player>(entt::get<C_CircleCollider, C_Origin, C_Position>).each())
                {
                        glm::vec2 player_center = glm::vec2(p_pos.position) + p_origin.origin;
                        float radius_sum_squared = (collider.radius + p_collider.radius) * (collider.radius + p_collider.radius);
                        glm::vec2 distance_vector = player_center - hitbox_center;
                        float distance_squared = (distance_vector.x * distance_vector.x) + (distance_vector.y * distance_vector.y);
                        if (distance_squared <= radius_sum_squared)
                        {
                                C_Velocity& vel = reg.get<C_Velocity>(p_entity);
                                vel.velocity += glm::vec3(hitbox.kb_dir, 0.0f);
                                //TODO: damage
                        }
                }
        }
}