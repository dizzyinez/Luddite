#include "systems/Camera.hpp"
#include "components/GameComponents.hpp"
#include "stb/stb_perlin.h"
#include "rendering/Renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include "core/game.hpp"

constexpr float CAMERA_SHAKE_COEFFICIENT = 70.0f;
constexpr float CAMERA_SHAKE_TIME_DILATION_COEFFICIENT = 5.0f;
constexpr float CAMERA_LERP_COEFFICIENT = 4.0f;

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
        Renderer::setCameraPosition(average_player_position);
        glm::vec2 mouse_dir = Renderer::screenToWorld(glm::vec2(Game::cursor_x, Game::cursor_y)) - average_player_position;

        mouse_dir.y *= Game::window_width / Game::window_height;
        if (glm::length2(mouse_dir) >= 0.0001)
        {
                float new_length = glm::length2(mouse_dir) * 0.00025f - 10.f;
                if (new_length < 0)
                        new_length = 0;
                if (new_length > 150.0f)
                        new_length = 150.0f;
                mouse_dir = glm::normalize(mouse_dir) * new_length;
        }

        glm::vec2 camera_to = average_player_position + mouse_dir;
        C_Camera& cam = reg.ctx<C_Camera>();
        cam.position += (camera_to - cam.position) * CAMERA_LERP_COEFFICIENT * deltaTime;
        glm::vec2 distance = camera_to - cam.position;
        cam.noise_timer += deltaTime;
        float final_cam_shake_coefficient = trauma * trauma * CAMERA_SHAKE_COEFFICIENT;
        glm::vec2 new_cam_position = cam.position + glm::vec2(
                final_cam_shake_coefficient * stb_perlin_fbm_noise3(cam.noise_timer * CAMERA_SHAKE_TIME_DILATION_COEFFICIENT, 0.f, 0.f, 2.1f, 0.5f, 3),
                final_cam_shake_coefficient * stb_perlin_fbm_noise3(0.f, cam.noise_timer * CAMERA_SHAKE_TIME_DILATION_COEFFICIENT, 0.f, 2.1f, 0.5f, 3)
                );
                
        Renderer::setCameraPosition(new_cam_position);
}