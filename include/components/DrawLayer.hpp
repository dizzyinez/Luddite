#ifndef  DrawLayer_hpp
#define  DrawLayer_hpp

enum class DrawLayer : uint8_t
{
        sprite,
        gui,
        count //used to easily give the total amount of layers, should not be used as a layer otherwise the game will crash!
};

struct C_DrawLayer
{
        DrawLayer layer {DrawLayer::sprite};
        C_DrawLayer() = default;
        C_DrawLayer(DrawLayer l) : layer(l)
        {
        }
};

#endif
