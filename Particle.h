#pragma once

#include <algorithm>
#include <iostream>

#include "vec2.h"
#include "Renderer.h"

class Particle
{
public:
  Particle(float x, float y, int radius);
  ~Particle();

  void Update(float gravity, float deltaTime);
  void Render(Renderer* renderer);

public:  
  float density;
  float pressure;
private:

  void KeepInBounds(float deltaTime);

public:
  vec2 pos;
  vec2 vel;
  int radius;
  float terminalVelocity = 5.0f;

public:
  float dampeningCoeff = 0.80f;
};
