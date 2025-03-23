#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

class Renderer
{
  public:
    Renderer(char* windowTitle, int width,int height);

    SDL_Window* GetWindow();    

    bool Init();

    void Clear();
    void Display();
    void Destroy();

    void DrawParticle(float x, float y, int radius);

  private:
    SDL_Window* window;

  public:
    SDL_GLContext context;

  private:
    char* windowName;
    int width;
    int height;
};
