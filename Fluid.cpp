#include "Fluid.h"

Fluid::Fluid(int width, int height, int particle_size)
  : fluidWidth(width), fluidHeight(height), particleSize(particle_size)
{
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_real_distribution<> dis(0.0, 1.0);

      double random_damp = dis(gen);

      int particle_index = particles.size() + 1;
      Particle* particle = new Particle(x + 100, y + 100, particle_size);
      particle->dampeningCoeff = random_damp;
      particles.push_back(particle);
    }
  } 
}

void Fluid::Update(float gravity, float deltaTime)
{
  for (auto& particle : particles)
    particle->Update(gravity, deltaTime);
}

void Fluid::Render(Renderer* renderer)
{
  for (auto& particle : particles)
    particle->Render(renderer);
}
