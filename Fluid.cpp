#include "Fluid.h"

Fluid::Fluid(int width, int height, int particle_size, float particle_mass)
  : fluidWidth(width), fluidHeight(height), particleSize(particle_size), particleMass(particle_mass)
{
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_real_distribution<> dis(0.9, 1.0);

      double random_damp = dis(gen);

      random_damp = 0.7;

      int particle_spacing = 10;

      int particle_index = particles.size() + 1;
      Particle* particle = new Particle(100 + x * particle_spacing, 100 + y * particle_spacing, particle_size, particle_mass);
      particle->dampeningCoeff = random_damp;
      particles.push_back(particle);
    }
  } 
}

void Fluid::Update(float gravity, float deltaTime)
{
  ComputeDensity();
  ComputePressure();
  ComputePressureForces();

  for (auto& particle : particles)
  {
    vec2 acceleration = particle->force / particle->mass;
    acceleration.y += gravity;

    particle->vel = particle->vel + acceleration * deltaTime;
    particle->pos = particle->pos + particle->vel * deltaTime;

    particle->KeepInBounds(deltaTime);
  }
}

void Fluid::Render(Renderer* renderer)
{
  float totalDensity = 0.0f;

  std::vector<std::pair<float, float>> positions;

  for (const auto& particle : particles)
  {
    totalDensity += particle->density;
    positions.emplace_back(particle->pos.x, particle->pos.y);
  }

  renderer->UpdateParticles(positions);

  float avgDensity = totalDensity / particles.size();
  std::cout << "Avg Density: " << avgDensity << std::endl;
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
        float W = (315.0f / (64.0f * M_PI * pow(h, 9))) * pow(h2 - r2, 3);
        p_i->density += W;
      }
    }
  }
}

void Fluid::ComputePressure()
{
  float restDensity = 100.0f;
  float stiffness = 200.0f;

  for (auto& p : particles)
  {
    p->pressure = stiffness * (p->density - restDensity);
  }
}

void Fluid::ComputePressureForces()
{
  float h = smoothingLength;
  float h2 = h * h;
  float mass = 1.0f;

  for (auto& p_i : particles)
  {
    vec2 force(0.0f, 0.0f);

    for (auto& p_j : particles)
    {
      if (p_i == p_j) continue;

      vec2 r_vec = p_i->pos - p_j->pos;
      float r2 = r_vec.dot(r_vec);

      if (r2 < h2)
      {
        float r = sqrt(r2);
        float W_grad = (-45.0f / (M_PI * std::pow(h, 6))) * std::pow(h - r, 2);

        vec2 pressureForce = r_vec.normalize() * (-mass * ((p_i->pressure + p_j->pressure) / 2.0f) * W_grad);
        force = force + pressureForce; 
      }
    }
    p_i->force = p_i->force + force;
  }
}
