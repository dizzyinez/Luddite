#include "systems/Draw.hpp"

#include <iostream>

#include "entt.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "rendering/Renderer.hpp"

#include "components/DrawLayer.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/Texture.hpp"
#include "components/Tileset.hpp"



bool sortbyheight(const std::pair<int,entt::entity> &a,
                  const std::pair<int,entt::entity> &b)
{
        //TODO: check if the entity is on the ground and loop through those first
        return (a.first < b.first);
}

void S_Draw::update(float dt, entt::registry &reg)
{
        //Animation
        reg.view<C_Animation>().each([&reg, dt](auto Entity, auto &animation){
                if (animation.animating)
                {
                        auto &tileset = reg.get<C_Tileset>(Entity);
                        animation.timer += dt;
                        if (animation.timer >= animation.seconds_per_frame)
                        {
                                animation.timer -= animation.seconds_per_frame;
                                if (animation.current_frame < animation.frames - 1)
                                {
                                        animation.current_frame++;
                                }
                                else if (animation.repeat) {
                                        animation.current_frame = 0;
                                } else
                                {
                                        animation.animating = false;
                                        animation.timer = 0.0;
                                }
                        }

                        tileset.index = tileset.tiles_width * animation.line + animation.current_frame;
                }
        });



        //Tilesets
        reg.group<C_Tileset>(entt::get<C_Sprite>).each([](auto Entity, auto &tileset, auto &sprite){
                sprite.tex_coords = glm::vec4(1.0f/(float)tileset.tiles_width  * (tileset.index % tileset.tiles_width),
                                              1.0f/(float)tileset.tiles_height * (tileset.index / tileset.tiles_width),
                                              1.0f/(float)tileset.tiles_width,
                                              1.0f/(float)tileset.tiles_height);
        });


        //Rendering
        std::vector<std::vector<std::pair<int, entt::entity> > > v(static_cast<int8_t>(DrawLayer::count));
        reg.group<C_DrawLayer>(entt::get<C_Position, C_Size>).each([&v](auto entity, auto &drawLayer, auto &pos, auto &size){
                v[static_cast<int8_t>(drawLayer.layer)].push_back(std::make_pair(pos.getY() + size.getW(), entity));
        });

        sort(v[static_cast<int8_t>(DrawLayer::sprite)].begin(), v[static_cast<int8_t>(DrawLayer::sprite)].end(), sortbyheight);       //sort the sprites by their height on the screen
        std::vector<std::vector<std::pair<int, entt::entity> > >::iterator layer;
        std::vector<std::pair<int, entt::entity> >::iterator pair;

        for ( layer = v.begin(); layer != v.end(); ++layer)
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
                        const auto [pos, size] = reg.get<C_Position, C_Size>(pair->second);
                        if (reg.has<C_Texture>(pair->second))
                        {
                                const auto tex = reg.get<C_Texture>(pair->second);
                                Renderer::RenderTexture(pos.position, size, tex.texture_id, tex.tex_coords);
                        }
                        if (reg.has<C_Sprite>(pair->second))
                        {
                                const auto sprite =  reg.get<C_Sprite>(pair->second);
                                Renderer::RenderSprite(pos.position, size, sprite.texture_id, sprite.tex_coords, sprite.colors);
                        }
                        // Renderer::RenderSprite(pos.position, size);
                        //TODO: textures
                }
                Renderer::flushSpriteBatch();
                Renderer::flushTextureBatch();
        }
}
