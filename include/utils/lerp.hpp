#pragma once

template <typename T, typename A>
T Lerp(const T& a1, const T& a2, const A& alpha)
{
        return a1 + ((a2 - a1) * alpha);
}