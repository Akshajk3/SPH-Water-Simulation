#include "Particle.h"

Particle::Particle(float x, float y, int radius, float mass)
  : pos(x, y), radius(radius), mass(mass)
{}

Particle::~Particle()
{}

void Particle::Update(float gravity, float deltaTime, int width, int height)
{
  float vertical_vel = std::min(vel.y + gravity * deltaTime, terminalVelocity);
  vel = vec2(0, vertical_vel);
  //std::cout << vertical_vel << std::endl;
  KeepInBounds(deltaTime, width, height);
  pos = pos + vel + force;
}

void Particle::KeepInBounds(float deltaTime, int width, int height)
{
  if (pos.x <= 0)
  {
    pos.x = 0;
    vel.x = -vel.x * dampeningCoeff;
  } 
  else if (pos.x >= width)
  {
    pos.x = width;
    vel.x = -vel.x * dampeningCoeff;
  }

  if (pos.y <= 0)
  {
    pos.y = 0;
    vel.y = -vel.y * dampeningCoeff;
  }
  else if (pos.y >= height)
  {
    pos.y = height;
    vel.y = -vel.y * dampeningCoeff;
  }
}
