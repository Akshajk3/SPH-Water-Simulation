#pragma once

#include <SDL2/SDL.h>
#include <iostream>

class Renderer
{
  public:
    Renderer(char* windowTitle, int width,int height);

    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();
    
    bool Init();

    void Clear();
    void Display();
    void Destroy();

  private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    char* windowName;
    int width;
    int height;
};
