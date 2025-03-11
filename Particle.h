#pragma once

#include "vec2.h"

class Particle
{
public:
  Particle(int x, int y, int radius);

  void Update(float gravity);
  void Render();

private:
  vec2 pos;
  vec2 vel;
  int radius;
};
