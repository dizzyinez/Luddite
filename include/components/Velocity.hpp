#ifndef Velocity_hpp
#define Velocity_hpp

#include "entityx/entityx.h"
#include "vec2.hpp"

struct Velocity : entityx::Component<Velocity>
{
        glm::vec3 velocity;
        //TODO: vec4 rotation
        Velocity(float x = 0, float y = 0) : velocity(x,y,0.0f){
        }
        Velocity(glm::vec3 vel) : velocity(vel){
        }

        float getX()
        {
                return velocity.x;
        }

        float getY()
        {
                return velocity.y;
        }

        void setX(float x)
        {
                velocity.x = x;
        }

        void setY(float y)
        {
                velocity.y = y;
        }

        //make add functions
};


#endif
