#include "systems/DrawLevel.hpp"
#include "rendering/Renderer.hpp"

void S_DrawLevel::update(float alpha, entt::registry &reg)
{
        Renderer::setProjectionWorld();
        Renderer::RenderLevel();
        // LevelRenderer::Render();
}