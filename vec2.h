#pragma once

#include <math.h>

struct vec2
{
    float x;
    float y;

    vec2()  : x(0), y(0)
    {};
    vec2(float x, float y) : x(x), y(y)
    {};
    ~vec2() = default;

    vec2 operator + (const vec2& other) const
    {
        return vec2(x + other.x, y + other.y);
    }

    vec2 operator + (float n) const
    {
        return vec2(x + n, y + n);
    }

    vec2 operator - (const vec2& other) const
    {
        return vec2(x - other.x, y - other.y);
    }

    vec2 operator * (float n) const
    {
        return vec2(x * n, y * n);
    }

    vec2 operator * (const vec2& other) const
    {
        return vec2(x * other.x, y * other.y);
    }

    vec2 operator / (float n) const
    {
        return vec2(x / n, y / n);
    }

    vec2 operator / (const vec2& other) const
    {
        return vec2(x / other.x, y / other.y);
    }

    static float magnitude(const vec2& vec)
    {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
    }
};
