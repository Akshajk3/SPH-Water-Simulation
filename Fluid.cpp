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

void Fluid::ComputeDensity()
{
  float h = smoothingLength;
  float h2 = h * h;

  for (auto& p_i : particles)
  {
    p_i->density = 0.0f;

    for (auto& p_j : particles)
    {
      vec2 r_vec = p_i->pos - p_j->pos;
      float r2 = r_vec.dot(r_vec);
      
      if(r2 < h2)
      {
        float r = std::sqrt(r2);
        float W = (315.0f)
      }
    }
  }
}
