#pragma once
#include <cstdint>
#include "kiwi/kiwi.h"
enum class constraints : uint8_t
{
        PIXEL,
        PERCENT,
        ASPECT,
        NONE
};

enum class edges : uint8_t
{
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
        CENTER
};

enum class dimensions : uint8_t
{
        WIDTH,
        HEIGHT,
        NONE
};


struct Constraint
{
        constraints type;
        edges edge;
        dimensions dimension;
        float data = 0;
};

Constraint CenterConstraint(dimensions d);

Constraint PixelConstriant(float pixel, edges edge = edges::CENTER);

Constraint PercentConstriant(float percent, dimensions d = dimensions::HEIGHT, edges edge = edges::CENTER);

Constraint AspectConstraint(float ratio);

struct C_Gui;
kiwi::Constraint create_constraint(C_Gui &Gui, C_Gui& Parent, int i);