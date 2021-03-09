#include "systems/Tileset.hpp"
#include "components/GameComponents.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

void S_Tilemap::update(float deltaTime, entt::registry &reg)
{
        reg.group<C_Tilemap>(entt::get<C_Sprite, C_Size>).each([](auto Entity, C_Tilemap &tilemap, auto &sprite, C_Size& size) {
                float off_x = 1.0f / float(sprite.texture->width);
                float off_y = 1.0f / float(sprite.texture->height);
                sprite.tex_coords = glm::vec4(
                        (float)tilemap.tilemap->get_x(tilemap.index) * off_x,
                        (float)tilemap.tilemap->get_y(tilemap.index) * off_y,
                        (float)tilemap.tilemap->get_w(tilemap.index) * off_x,
                        (float)tilemap.tilemap->get_h(tilemap.index) * off_y

                        // 1.0f / ((float)tilemap.tilemap->get_x(tilemap.index)) + off_x,
                        // 1.0f / ((float)tilemap.tilemap->get_y(tilemap.index)) + off_y,
                        // 1.0f / ((float)tilemap.tilemap->get_w(tilemap.index)) - off_x,
                        // 1.0f / ((float)tilemap.tilemap->get_h(tilemap.index)) - off_y
                        // 1.0f / (float)tileset.tiles_width * (tileset.index % tileset.tiles_width) + off_x,
                        // 1.0f / (float)tileset.tiles_height * (tileset.index / tileset.tiles_width) + off_y,
                        // 1.0f / ((float)tileset.tiles_width) - off_y,
                        // 1.0f / ((float)tileset.tiles_height - off_y)
                        );
                // std::cout << (int)tilemap.tilemap->get_x(tilemap.index) << std::endl;
                size.size.x = tilemap.tilemap->get_w(tilemap.index);
                size.size.y = tilemap.tilemap->get_h(tilemap.index);
                sprite.origin.x = tilemap.tilemap->get_origin_x(tilemap.index);
                sprite.origin.y = tilemap.tilemap->get_origin_y(tilemap.index);
        });
}
