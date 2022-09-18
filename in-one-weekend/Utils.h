#pragma once
#define _USE_MATH_DEFINES
#include "Common.h"
#include <random>
#include <math.h>

inline f32 to_rad(f32 degrees)
{
    return degrees * (f32)M_PI / 180.f;
}

f32 random_float(f32 min, f32 max);

// Float between 0.0 and 1.0
inline f32 random_float()
{
    return (f32)(rand() / (RAND_MAX + 1.0));

    static std::uniform_real_distribution<f32> distribution(0.f, 1.f);
    static std::mt19937 generator;
    return distribution(generator);
}

inline f32 clamp(f32 x, f32 min, f32 max) 
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
