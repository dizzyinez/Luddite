#ifndef LayerBase_hpp
#define LayerBase_hpp

#include "layers/Layer.hpp"

class L_Base : public Layer
{
public:
        void init();
        void handleEvents(float deltaTime);
        void update(float deltaTime);
        void render(float alpha);
        void clean();
        L_Base();
};

#endif
