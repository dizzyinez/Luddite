#include "systems/Tileset.hpp"
#include "components/Tileset.hpp"
#include "components/Texture.hpp"
#include <glm/glm.hpp>

void S_Tileset::update(float deltaTime, entt::registry &reg)
{
        reg.group<C_Tileset>(entt::get<C_Sprite>).each([](auto Entity, auto &tileset, auto &sprite) {
                sprite.tex_coords = glm::vec4(1.0f / (float)tileset.tiles_width * (tileset.index % tileset.tiles_width),
                        1.0f / (float)tileset.tiles_height * (tileset.index / tileset.tiles_width),
                        1.0f / (float)tileset.tiles_width,
                        1.0f / (float)tileset.tiles_height);
        });
}
