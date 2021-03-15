#ifndef Position_hpp
#define Position_hpp

#include <glm/vec3.hpp>

struct C_Position
{
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        //TODO: vec(4?) rotation
        C_Position() = default;
        C_Position(float x, float y) : position(x, y, 0.0f) {
        }
        C_Position(glm::vec3 pos) : position(pos) {
        }

        float getX() const
        {
                return position.x;
        }

        float getY() const
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
        operator const glm::vec3&() {return position;}
};
#endif
