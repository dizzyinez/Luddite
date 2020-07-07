#ifndef  Drawable_hpp
#define  Drawable_hpp

#include "entityx/entityx.h"

enum class Layer : uint8_t
{
        sprite,
        gui,
        count //used to easily give the total amount of layers, should not be used as a layer otherwise the game will crash
};

struct Drawable : entityx::Component<Drawable>
{
public:
        Drawable() : layer(Layer::sprite)
        {
        }
        Drawable(Layer l) : layer(l)
        {
        }
        Layer layer;

};

#endif
