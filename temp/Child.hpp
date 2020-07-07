#include "vec2.hpp"

struct Child_Type
{
        virtual vec2f Interpolation(vec2f a, vec2f b) = 0;
};

struct Child_Type_Direct : public Child_Type
{
        Child_Type_Direct()
        {

        }
        vec2f Interpolation(vec2f a, vec2f b)
        {
                return b;
        }
};


struct Child_Type_Lerp : public Child_Type
{
        float percent;
        Child_Type_Lerp(float p) : percent(p)
        {

        }
        vec2f Interpolation(vec2f a, vec2f b)
        {
                vec2f c = b-a;
                c *= percent;
                return a + c;
        }
};

struct Child_Type_Lerp_Bounds : public Child_Type
{
        float percent, lower_bound, upper_bound;
        Child_Type_Lerp_Bounds(float p, float ub) : percent(p), lower_bound(0), upper_bound(ub)
        {
        }
        Child_Type_Lerp_Bounds(float p, float lb, float ub) : percent(p), lower_bound(lb), upper_bound(ub)
        {
        }
        vec2f Interpolation(vec2f a, vec2f b)
        {
                float d = b.dist(a);
                vec2f c = b-a;
                if (d > upper_bound)
                {
                        c.truncate(upper_bound);
                        return a + c;
                }
                if (d < lower_bound)
                {
                        c.truncate(lower_bound);
                        return a + c;
                }
                c *= percent;
                return a + c;
        }
};

//lerp
//lerp w/ distance cap
