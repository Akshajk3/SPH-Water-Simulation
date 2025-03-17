#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <random>

#include "Renderer.h"
#include "Particle.h"

class Fluid
{
public:
  Fluid(int width, int height, int particle_size);

  void Update(float gravity, float deltaTime);
  void Render(Renderer* renderer);
  void ComputeDensity();
private:
  int fluidWidth;
  int fluidHeight;
  int particleSize;

  std::vector<Particle*> particles;
};
