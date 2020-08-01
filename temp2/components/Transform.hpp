#ifndef Transform_hpp
#define Transform_hpp

#include "entityx/entityx.h"
#include <glm/vec3.hpp>

struct Transform : entityx::Component<Transform>
{
        glm::vec3 position;
        //TODO: vec4 rotation
        Transform(float x = 0, float y = 0) : position(x,y,0.0f){
        }
        Transform(glm::vec3 pos) : position(pos){
        }

        float getX()
        {
                return position.x;
        }

        float getY()
        {
                return position.y;
        }

        void setX(float x)
        {
                position.x = x;
        }

        void setY(float y)
        {
                position.y = y;
        }

        //make add functions
};

#endif
