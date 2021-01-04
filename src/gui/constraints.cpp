#include "gui/constraints.hpp"
#include "components/Gui.hpp"

Constraint CenterConstraint(dimensions d)
{
        return {constraints::PERCENT, edges::CENTER, d, 0.5f};
}

Constraint PixelConstriant(float pixel, edges edge)
{
        return {constraints::PIXEL, edge, dimensions::WIDTH, pixel};
}

Constraint PercentConstriant(float percent, dimensions d, edges edge)
{
        return {constraints::PERCENT, edge, d, percent};
}

Constraint AspectConstraint(float ratio)
{
        return {constraints::ASPECT, edges::CENTER, dimensions::WIDTH, ratio};
}

kiwi::Constraint create_constraint(C_Gui &Gui, C_Gui& Parent, int i)
{
        Constraint& c = Gui.constraints[i];

        // kiwi::Constraint constraint;
        // kiwi::Variable& var = (i == 0) ? Gui.x : (i == 1) ? Gui.y : (i == 2) ? Gui.w : Gui.h;
        switch (c.type)
        {
        case  constraints::PIXEL:
                switch (c.edge)
                {
                case edges::LEFT:
                        return {Gui.x == Parent.x + Gui.xd + c.data};
                        break;

                case edges::TOP:
                        return {Gui.y == Parent.y + Gui.yd + c.data};
                        break;

                case edges::RIGHT:
                        return {Gui.x + Gui.w == Parent.x + Gui.xd + Parent.w - c.data};
                        break;

                case edges::BOTTOM:
                        return {Gui.y + Gui.h == Parent.y + Gui.yd + Parent.h - c.data};
                        break;

                case edges::CENTER:
                        if (i == 0) return {Gui.x + (Gui.w / 2) == Parent.x + Gui.xd + c.data};
                        if (i == 1) return {Gui.y + (Gui.h / 2) == Parent.y + Gui.yd + c.data};
                        if (i == 2) return {Gui.w == Parent.x + Gui.wd + c.data};
                        if (i == 3) return {Gui.h == Parent.y + Gui.hd + c.data};
                        break;
                }
                break;

        case  constraints::PERCENT:
        {
                kiwi::Variable& dimension = (c.dimension == dimensions::HEIGHT ?  Parent.h : Parent.w);
                switch (c.edge)
                {
                case edges::LEFT:
                        return {Gui.x == Parent.x + Gui.xd + (c.data * dimension)};
                        break;

                case edges::TOP:
                        return {Gui.y == Parent.y + Gui.yd + (c.data * dimension)};
                        break;

                case edges::RIGHT:
                        return {Gui.x + Gui.w == Parent.x + Gui.xd + Parent.w - (c.data * dimension)};
                        break;

                case edges::BOTTOM:
                        return {Gui.y + Gui.h == Parent.y + Gui.yd + Parent.h - (c.data * dimension)};
                        break;

                case edges::CENTER:
                        if (i == 0) return {Gui.x + (Gui.w / 2) == Parent.x + Gui.xd + (c.data * dimension)};
                        if (i == 1) return {Gui.y + (Gui.h / 2) == Parent.y + Gui.yd + (c.data * dimension)};
                        if (i == 2) return {Gui.w == Gui.wd + (c.data * dimension)};
                        if (i == 3) return {Gui.h == Gui.hd + (c.data * dimension)};
                        break;
                }
        }
        break;

        case constraints::ASPECT:
                if (i == 2) {return {Gui.w == Gui.wd + c.data * Gui.h};}
                if (i == 3) {return {Gui.h == Gui.hd + (1.0f / c.data) * Gui.w};}
                break;
        }
        //yer fuckin constriant is invalid!
        assert(0);
}