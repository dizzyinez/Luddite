#include "systems/Hitboxes.hpp"
#include "components/GameComponents.hpp"
#include <iostream>
#include "core/AudioPlayer.hpp"

void S_Hitboxes::update(float deltaTime, entt::registry &reg)
{
        for (auto &&[entity, hitbox, team, collider, pos]: reg.group<C_Hitbox, C_Team>(entt::get<C_CircleCollider, C_Position>).each())
        {
                if (hitbox.enabled)
                {
                        glm::vec2 hitbox_center = glm::vec2(pos.position);
                        for (auto &&[p_entity, p_collider, p_pos, p_team, p_vel]: reg.group<C_CircleCollider>(entt::get<C_Position, C_Team, C_Velocity>).each())
                        {
                                if (team.team != p_team.team)
                                {
                                        glm::vec2 player_center = glm::vec2(p_pos.position);
                                        float radius_sum_squared = (collider.radius + p_collider.radius) * (collider.radius + p_collider.radius);
                                        glm::vec2 distance_vector = player_center - hitbox_center;
                                        float distance_squared = (distance_vector.x * distance_vector.x) + (distance_vector.y * distance_vector.y);
                                        if (distance_squared <= radius_sum_squared)
                                        {
                                                AudioPlayer::PlaySound(SoundAllocator::Get("../assets/sounds/hit1.wav"));
                                                // C_Velocity& vel = reg.get<C_Velocity>(p_entity);
                                                // p_vel.velocity += glm::vec3(hitbox.kb_dir, 0.0f);
                                                if (reg.has<C_Player>(p_entity))
                                                        reg.get<C_Player>(p_entity).trauma += 1.f;
                                                //TODO: damage
                                        }
                                }
                        }
                }
        }
}