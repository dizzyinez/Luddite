#ifndef L_Networking_hpp
#define L_Networking_hpp

#include "Layers/Layer.hpp"

class L_Networking : public Layer
{
public:
        void init();
        void handleEvents(float deltaTime);
        void update(float deltaTime);
        void render(float alpha);
        void clean();
        L_Networking();
};


#endif
