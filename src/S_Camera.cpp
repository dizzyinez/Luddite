#include "systems/Camera.hpp"
#include "components/GameComponents.hpp"
#include "stb/stb_perlin.h"
#include "rendering/Renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

constexpr float CAMERA_SHAKE_COEFFICIENT = 35.0f;
constexpr float CAMERA_SHAKE_TIME_DILATION_COEFFICIENT = 2.5f;
constexpr float CAMERA_LERP_COEFFICIENT = 3.0f;

void S_Camera::update(float alpha, float deltaTime, entt::registry& lerp_reg, entt::registry& reg)
{
        float trauma = 0;
        glm::vec2 average_player_position = glm::vec2(0.f);
        float count = 0;
        for (auto &&[Entity, player] : lerp_reg.view<C_Player>().each())
        {
                if (player.local_player)
                {
                        trauma += player.trauma;
                        average_player_position += glm::vec2(reg.get<C_Position>(Entity).position);
                        count += 1.f;
                }
        }
        average_player_position /= count;
        trauma /= count;
        C_Camera& cam = reg.ctx<C_Camera>();
        cam.position += (average_player_position - cam.position) * CAMERA_LERP_COEFFICIENT * deltaTime;
        glm::vec2 distance = average_player_position - cam.position;
        cam.noise_timer += deltaTime;
        float final_cam_shake_coefficient = trauma * trauma * CAMERA_SHAKE_COEFFICIENT;
        glm::vec2 new_cam_position = cam.position + glm::vec2(
                final_cam_shake_coefficient * stb_perlin_fbm_noise3(cam.noise_timer * CAMERA_SHAKE_TIME_DILATION_COEFFICIENT, 0.f, 0.f, 2.1f, 0.5f, 6),
                final_cam_shake_coefficient * stb_perlin_fbm_noise3(0.f, cam.noise_timer * CAMERA_SHAKE_TIME_DILATION_COEFFICIENT, 0.f, 2.1f, 0.5f, 6)
                );
        Renderer::setCameraPosition(new_cam_position);
}