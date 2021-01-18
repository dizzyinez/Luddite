#ifndef Layer_hpp
#define Layer_hpp

//#include "game.h"
#include <stack>
#include "entt.hpp"
#include "systems/System.hpp"

class Game;
class Entity;

class Layer
{
public:
        Game* game;


        virtual void init() {}
        virtual void handleEvents(float deltaTime) {}
        virtual void update(float deltaTime) {}
        virtual void render(float alpha) {}
        virtual void clean() {}

        Entity CreateEntity();
        Entity ToEntity(entt::entity);
        template <typename T>
        T& GetContext()
        {
                return m_Registry.ctx<T>();
        }
        template <typename T, typename ... Args>
        T& SetContext(Args && ... args)
        {
                return m_Registry.set<T>(std::forward<Args>(args)...);
        }

protected:
        entt::registry m_Registry;
        entt::registry last_frame;
        entt::registry lerp_frame;
        SystemManager systems;
        friend class Entity;
        friend class load_screen;
};

#endif
