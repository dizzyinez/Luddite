#ifndef Size_hpp
#define Size_hpp
#include "entityx/entityx.h"
#include <glm/vec2.hpp>

struct Size : entityx::Component<Size>
{
        glm::vec2 size;
        Size(float x = 0, float y = 0) : size(x,y){
        }
        Size(glm::vec2 s) : size(s){
        }

        float getX()
        {
                return size.x;
        }

        float getY()
        {
                return size.y;
        }

        void setW(float w)
        {
                size.x = w;
        }

        void setH(float h)
        {
                size.y = h;
        }

        //make add functions
};

#endif
