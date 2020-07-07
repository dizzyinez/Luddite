#include "systems/DrawSystem.hpp"

#include "components/Drawable.hpp"
#include "components/Transform.hpp"
#include "components/Size.hpp"
// #include "components/Texture.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Locator.hpp"
#include <iostream>

struct Layers
{
        static const int main = 0;
};

bool sortbyheight(const std::pair<int,int> &a,
                  const std::pair<int,int> &b)
{
        //TODO: check if the entity is on the ground and loop through those first
        return (a.first > b.first);
}

void DrawSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
        std::vector<std::vector<std::pair<int, entityx::Entity> > > v(static_cast<int8_t>(Layer::count));
        for (auto entity : es.entities_with_components<Drawable, Transform>())
        {
                entityx::ComponentHandle<Drawable> drawable = entity.component<Drawable>();
                entityx::ComponentHandle<Transform> transform = entity.component<Transform>();
                if (drawable && transform) {
                        //TODO: rotation
                        v[static_cast<int8_t>(drawable->layer)].push_back(std::make_pair(transform->getY(), entity));   //add height to pos
                }
        }

        sort(v[static_cast<int8_t>(Layer::sprite)].begin(), v[static_cast<int8_t>(Layer::sprite)].end(), sortbyheight); //sort the sprites by their height on the screen
        std::vector<std::vector<std::pair<int, entityx::Entity> > >::iterator layer;
        std::vector<std::pair<int, entityx::Entity> >::iterator pair;

        for ( layer = v.begin(); layer != v.end(); ++layer)
        {
                //set the correct view projection matrix for each layer
                switch (std::distance(v.begin(), layer))
                {
                case static_cast<int8_t>(Layer::sprite):
                        Locator::getRenderer()->setProjectionWorld();
                        break;
                case static_cast<int8_t>(Layer::gui):
                        Locator::getRenderer()->setProjectionScreen();
                        break;
                }

                for (pair = layer->begin(); pair != layer->end(); ++pair)
                {
                        entityx::ComponentHandle<Transform> transform = pair->second.component<Transform>();
                        entityx::ComponentHandle<Size> size = pair->second.component<Size>();
                        if (size)
                        {
                                // std::cout << "rendering " << std::endl;
                                Locator::getRenderer()->RenderSprite(transform->position, size->size); //maybe? have a seperate position component which is a vec2 in order to save a tiny bit of memory
                        }
                        else
                        {
                                Locator::getRenderer()->RenderSprite(transform->position, glm::vec2(200.0f, 200.0f)); //draws the object with a default size
                        }
                        // entityx::ComponentHandle<Texture> texture = pair->second.component<Texture>();
                        // if (texture)
                        // {
                        //
                        // }
                }
                Locator::getRenderer()->flushSpriteBatch();
        }

}
