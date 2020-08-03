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
// // #include "components/Texture.hpp"



bool sortbyheight(const std::pair<int,entt::entity> &a,
                  const std::pair<int,entt::entity> &b)
{
        //TODO: check if the entity is on the ground and loop through those first
        return (a.first > b.first);
}

void S_Draw::update(float dt, entt::registry &reg)
{
        std::vector<std::vector<std::pair<int, entt::entity> > > v(static_cast<int8_t>(DrawLayer::count));
        reg.group<C_DrawLayer>(entt::get<C_Position, C_Size>).each([&v](auto entity, auto &drawLayer, auto &pos, auto &size){
                v[static_cast<int8_t>(drawLayer.layer)].push_back(std::make_pair(pos.getY(), entity));
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
                        Renderer::RenderSprite(pos.position, size);
                        //TODO: textures
                }
                Renderer::flushSpriteBatch();
        }
}
