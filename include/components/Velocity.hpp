#ifndef Velocity_hpp
#define Velocity_hpp

#include <glm/vec3.hpp>

struct C_Velocity
{
        glm::vec3 velocity{0.0f, 0.0f, 0.0f};
        C_Velocity() = default;
        C_Velocity(float x, float y) : velocity(x,y,0.0f){
        }
        C_Velocity(glm::vec3 &vel) : velocity(vel){
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
        operator const glm::vec3&() {return velocity;}

        //make add functions
};


#endif
