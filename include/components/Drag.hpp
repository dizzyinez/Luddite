#ifndef C_Drag_hpp
#define C_Drag_hpp

struct C_Drag
{
        C_Drag() = default;
        C_Drag(float drag_)
                : drag(drag_)
        {
        }
        float drag = 0.99;
        operator const float&()
        {
                return drag;
        }
};

#endif
