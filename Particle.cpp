#include "Particle.h"

Particle::Particle(float x, float y, int radius, float mass)
  : pos(x, y), radius(radius), mass(mass)
{}

Particle::~Particle()
{}

void Particle::Update(float gravity, float deltaTime)
{
  float vertical_vel = std::min(vel.y + gravity * deltaTime, terminalVelocity);
  vel = vec2(0, vertical_vel);
  //std::cout << vertical_vel << std::endl;
  KeepInBounds(deltaTime);
  pos = pos + vel + force;
}

void Particle::Render(Renderer* renderer)
{
  renderer->DrawParticle(pos.x, pos.y, radius);
}

void Particle::KeepInBounds(float deltaTime)
{
  if (pos.x <= 0)
  {
    pos.x = 0;
    vel.x = -vel.x * dampeningCoeff;
  } 
  else if (pos.x >= 800)
  {
    pos.x = 800;
    vel.x = -vel.x * dampeningCoeff;
  }

  if (pos.y <= 0)
  {
    pos.y = 0;
    vel.y = -vel.y * dampeningCoeff;
  }
  else if (pos.y >= 600)
  {
    pos.y = 600;
    vel.y = -vel.y * dampeningCoeff;
  }
}
