#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <random>

#include <GL/glew.h>

#include "Renderer.h"
#include "Particle.h"
#include "HashGrid.h"

class Fluid
{
public:
  Fluid(int width, int height, int particle_size, float particle_mass, int screen_width, int screen_height);

  void Update(float deltaTime);
  void Render(Renderer* renderer);
  void ComputeDensity();
  void ComputePressure();
  void ComputePressureForces();

private:
  std::string GetShaderSource(const char* filename);
  GLuint CompileShader(const char* shaderPath);

public:
  void UpdateWindowBounds(int width, int height);

public:
  int fluidWidth;
  int fluidHeight;
  int particleSize;
  float particleMass;
  float gravity = 500.0f;
  float restDensity = 80.0f;
  float stiffness = 500.0f;
  float repulsionStrength = 1.0f;
  float attractionStrength = 1.0f;
  float viscosity = 0.1f;
  float smoothingLength = 20.0f;

  float avgDensity = 0.0f;

  int WIDTH;
  int HEIGHT;

  std::vector<Particle*> particles;
  HashGrid hashGrid;
};
