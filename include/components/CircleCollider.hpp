#pragma once

struct C_CircleCollider
{
        C_CircleCollider() = default;
        C_CircleCollider(float radius_) : radius(radius_) {}
        float radius = 0.0f;
};