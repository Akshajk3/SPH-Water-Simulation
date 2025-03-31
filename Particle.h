#pragma once

#include <algorithm>
#include <iostream>

#include "vec2.h"
#include "Renderer.h"

class Particle
{
public:
  Particle(float x, float y, int radius, float mass);
  ~Particle();

  void Update(float gravity, float deltaTime, int width, int height);

public:  
  float density;
  float pressure;
  vec2 force;
  float mass = 1.0f;

public:
  void KeepInBounds(float deltaTime, int width, int height);

public:
  vec2 pos;
  vec2 vel;
  int radius;
  float terminalVelocity = 5.0f;

public:
  float dampeningCoeff = 0.80f;
};
