#pragma once

#include <iostream>
#include <SDL2/SDL.h>

class Fluid
{
public:
  Fluid(int width, int height, int particle_size);

  void Update();
  void Render();

private:
  int fluidWidth;
  int fluidHeight;
  int particleSize;
};
