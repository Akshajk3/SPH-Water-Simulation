#include <iostream>
#include <SDL2/SDL.h>

#include "Renderer.h"
#include "Fluid.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;

int main(int argc, char** argv)
{
  if (SDL_Init(SDL_INIT_VIDEO) > 0)
  {
    std::cout << "Failed to init SDL_Video: " << SDL_GetError() << std::endl;
  }


  Renderer renderer("Flip Water Sim", WIDTH, HEIGHT);

  Fluid fluid(10, 10, 3);

  bool running = true;

  if (!renderer.Init())
  {
    running = false;
  }

  SDL_Event e;

  Uint32 frameStart;
  float deltaTime = 0.0;

  while (running)
  {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
        running = false;
      
      if (e.type == SDL_KEYDOWN)
      {
        if (e.key.keysym.sym == SDLK_ESCAPE)
          running = false; 
      }
    }

    renderer.Clear();
    fluid.Update(80, deltaTime);
    fluid.Render(&renderer);
    renderer.Display();

    deltaTime = (SDL_GetTicks() - frameStart) / 1000.0f;

    int frameTime = SDL_GetTicks() - frameStart;
    if (1000 / FPS > frameTime) 
    {
      SDL_Delay(1000/FPS - frameTime);
    }
  }

  renderer.Destroy();
  SDL_Quit();
  return 0;
}
