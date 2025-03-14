#include "Fluid.h"

Fluid::Fluid(int width, int height, int particle_size)
  : fluidWidth(width), fluidHeight(height), particleSize(particle_size)
{
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      int particle_index = particles.size() + 1;
      particles.push_back(Particle(x + 100, y + 100, particle_size));
    }
  }
}

void Fluid::Update(float gravity, float deltaTime)
{
  for (auto particle : particles)
    particle.Update(gravity, deltaTime);
}

void Fluid::Render(Renderer* renderer)
{
  for (auto particle : particles)
    particle.Render(renderer);
}
