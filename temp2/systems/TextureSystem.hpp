#ifndef TextureSystem_hpp
#define TextureSystem_hpp

#include "entityx/entityx.h"

struct TextureSystem : public entityx::System<TextureSystem>
{
public:
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
private:
        //TextureAllocator TA;
};

#endif
