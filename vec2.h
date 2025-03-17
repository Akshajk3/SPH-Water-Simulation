#pragma once

#include <math.h>
#include <algorithm>

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

  bool operator <= (const vec2& other) const
  {
    return magnitude(*this) <= magnitude(other);
  }

  bool operator >= (const vec2& other) const
  {
    return magnitude(*this) >= magnitude(other);
  }

  bool operator < (const vec2& other) const
  {
    return magnitude(*this) < magnitude(other);
  }

  bool operator > (const vec2& other) const
  {
    return magnitude(*this) > magnitude(other);
  }

  vec2 min (const vec2& other) const
  {
    return vec2(std::min(x, other.x), std::min(y, other.y));
  } 

  vec2 max (const vec2& other) const
  {
    return vec2(std::max(x, other.x), std::max(y, other.y));
  }

  static float magnitude(const vec2& vec)
  {
      return sqrt(vec.x * vec.x + vec.y * vec.y);
  }
  
  float dot(const vec2& vec)
  {
    return x * vec.x + y * vec.y;
  }
};
