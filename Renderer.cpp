#include "Renderer.h"

Renderer::Renderer(char* windowTitle, int width, int height)
  :  windowName(windowTitle), width(width), height(height)
{}

bool Renderer::Init()
{
  window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
  if (window == nullptr)
  {
    std::cout << "Failed to Initialze Window, Error: " << SDL_GetError() << std::endl;
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr)
  {
    std::cout << "Failed to Initialze renderer, Error: " << SDL_GetError() << std::endl;
    return false;
  }

  return true;
}

void Renderer::Clear()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}

void Renderer::Display()
{
  SDL_RenderPresent(renderer);
}

void Renderer::Destroy()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void Renderer::DrawParticle(int x_center, int y_center, int radius)
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  int x = radius;
  int y = 0;
  int decision = 1 - radius;

  while (x >= y)
  {
    SDL_RenderDrawLine(renderer, x_center - x, y_center + y, x_center + x, y_center + y);
    SDL_RenderDrawLine(renderer, x_center - x, y_center - y, x_center + x, y_center - y);
    SDL_RenderDrawLine(renderer, x_center - y, y_center + x, x_center + y, y_center + x);
    SDL_RenderDrawLine(renderer, x_center - y, y_center - x, x_center + x, y_center - x);

    y++;

    if (decision <= 0)
    {
      decision += 2 * y + 1;
    }
    else
    {
      x--;
      decision += 2 * (y - x) + x;
    }
  }
}
