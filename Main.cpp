#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>

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

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  Renderer renderer("Flip Water Sim", WIDTH, HEIGHT, "../shaders/vert.glsl", "../shaders/frag.glsl");

  Fluid fluid(10, 10, 3, 10.0);

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
    fluid.Update(100, deltaTime);
    fluid.Render(&renderer);
    renderer.Display();

    deltaTime = (SDL_GetTicks() - frameStart) / 1000.0f;

    int frameTime = SDL_GetTicks() - frameStart;

    int frameRate = 1000 / frameTime;
    std::string windowName = "Flip Water Sim FPS: " + std::to_string(frameRate);
    SDL_SetWindowTitle(renderer.GetWindow(), windowName.c_str());

    if (1000 / FPS > frameTime) 
    {
      SDL_Delay(1000/FPS - frameTime);
    }
  }

  renderer.Destroy();
  SDL_Quit();
  return 0;
}
