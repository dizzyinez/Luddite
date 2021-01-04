#include "systems/Draw.hpp"

#include <iostream>

#include "entt.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

#include "rendering/Renderer.hpp"

#include "components/DrawLayer.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/Texture.hpp"
#include "components/Tileset.hpp"



bool sortbyheight(const std::pair<int, entt::entity> &a,
                  const std::pair<int, entt::entity> &b)
{
        return(a.first < b.first);
        return false;
}
// bool sortbydepth(const std::pair<int, entt::entity> &a,
//                  const std::pair<int, entt::entity> &b)
// {
//         return(a.first > b.first);
//         return false;
// }

void S_Draw::update(float alpha, entt::registry &reg)
{
        //vector of vector of pairs of entities and their depth positions
        std::vector<std::vector<std::pair<int, entt::entity> > > v(static_cast<int8_t>(DrawLayer::count));

        reg.group<C_DrawLayer>(entt::get<C_Position, C_Size>).each([&v, &reg, alpha](auto entity, C_DrawLayer &drawLayer, auto &pos, auto &size) {
                int depth = 0;
                if (drawLayer.layer == DrawLayer::sprite)
                        depth = pos.getY() + size.getH();
                //TODO ^^^^ add z ???
                if (drawLayer.layer == DrawLayer::gui)
                        depth = drawLayer.depth;
                v[static_cast<int8_t>(drawLayer.layer)].push_back(std::make_pair(depth, entity));
        });

        //sort the sprites by their height on the screen
        sort(v[static_cast<int8_t>(DrawLayer::sprite)].begin(), v[static_cast<int8_t>(DrawLayer::sprite)].end(), sortbyheight);
        sort(v[static_cast<int8_t>(DrawLayer::gui)].begin(), v[static_cast<int8_t>(DrawLayer::gui)].end(), sortbyheight);
        std::vector<std::vector<std::pair<int, entt::entity> > >::iterator layer;
        std::vector<std::pair<int, entt::entity> >::iterator pair;

        for (layer = v.begin(); layer != v.end(); ++layer)
        {
                //set the correct view projection matrix for each layer
                switch (std::distance(v.begin(), layer))
                {
                case static_cast<int8_t>(DrawLayer::sprite):
                        Renderer::setProjectionWorld();
                        break;

                case static_cast<int8_t>(DrawLayer::gui):
                        Renderer::setProjectionScreen();
                        break;
                }

                for (pair = layer->begin(); pair != layer->end(); ++pair)
                {
                        glm::vec3 &object_position = reg.get<C_Position>(pair->second).position;
                        glm::vec2 &object_size = reg.get<C_Size>(pair->second).size;
                        // const auto [pos, size] = reg.get<C_Position, C_Size>(pair->second);
                        if (reg.has<C_Texture>(pair->second))
                        {
                                // std::cout << "rendering texture" << std::endl;
                                const auto tex = reg.get<C_Texture>(pair->second);
                                if (reg.has<C_Tint>(pair->second))
                                        Renderer::RenderTexture(object_position, object_size, tex.texture->texture_id, tex.tex_coords, reg.get<C_Tint>(pair->second).tint);
                                else
                                        Renderer::RenderTexture(object_position, object_size, tex.texture->texture_id, tex.tex_coords);
                        }
                        else if (reg.has<C_Sprite>(pair->second))
                        {
                                const auto sprite = reg.get<C_Sprite>(pair->second);
                                Renderer::RenderSprite(object_position, object_size, sprite.texture->texture_id, sprite.tex_coords, sprite.colors);
                        }
                        else
                        {
                                if (reg.has<C_Tint>(pair->second))
                                        Renderer::RenderRect(object_position, object_size, reg.get<C_Tint>(pair->second).tint);
                                else
                                        Renderer::RenderRect(object_position, object_size);
                        }
                        // Renderer::RenderSprite(pos.position, size);
                        //TODO: textures
                }
                Renderer::flushSpriteBatch();
                Renderer::flushTextureBatch();
        }
}
