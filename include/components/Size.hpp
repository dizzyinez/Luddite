#ifndef Size_hpp
#define Size_hpp
#include <glm/vec2.hpp>

struct C_Size
{
        glm::vec2 size{0.0f, 0.0f};
        C_Size() = default;
        C_Size(float x, float y) : size(x,y){
        }
        C_Size(glm::vec2 &s) : size(s){
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

        operator const glm::vec2& () {return size;}
};

#endif
