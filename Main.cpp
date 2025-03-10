#include <iostream>
#include <SDL2/SDL.h>

#include "Renderer.h"

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO) > 0)
  {
    std::cout << "Failed to init SDL_Video: " << SDL_GetError() << std::endl;
  }


  Renderer renderer("Flip Water Sim", 800, 800);

  bool running = true;

  if (!renderer.Init())
  {
    running = false;
  }

  SDL_Event e;

  while (running)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
        running = false;
    }

    renderer.Clear();
    renderer.Display();
  }

  renderer.Destroy();
  SDL_Quit();
  return 0;
}
