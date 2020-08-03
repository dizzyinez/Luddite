#ifndef LayerBase_hpp
#define LayerBase_hpp

#include "Layers/Layer.hpp"

class L_Base : public Layer
{
public:
void init();
void handleEvents(float deltaTime);
void update(float deltaTime);
void render(float deltaTime);
void clean();
L_Base();
};

#endif
