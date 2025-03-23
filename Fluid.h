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
  Fluid(int width, int height, int particle_size, float particle_mass);

  void Update(float gravity, float deltaTime);
  void Render(Renderer* renderer);
  void ComputeDensity();
  void ComputePressure();
  void ComputePressureForces();
private:
  int fluidWidth;
  int fluidHeight;
  int particleSize;
  float particleMass;
  
  float smoothingLength = 12;

  std::vector<Particle*> particles;
};
