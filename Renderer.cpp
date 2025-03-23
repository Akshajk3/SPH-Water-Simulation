#include "Renderer.h"

Renderer::Renderer(char* windowTitle, int width, int height)
  :  windowName(windowTitle), width(width), height(height)
{}

bool Renderer::Init()
{
  window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (window == nullptr)
  {
    std::cout << "Failed to Initialze Window, Error: " << SDL_GetError() << std::endl;
    return false;
  }

  context = SDL_GL_CreateContext(window);
  if (context == nullptr)
  {
    std::cout << "Failed to create OpenGL context, Error: " << SDL_GetError() << std::endl;
    return false; 
  }

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW\n";
    return false;
  }

  return true;
}

void Renderer::Clear()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Display()
{
  SDL_GL_SwapWindow(window);
}

void Renderer::Destroy()
{
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
}

void Renderer::DrawParticle(float x_center, float y_center, int radius)
{
  glColor3f(1.0f, 1.0f, 1.0f);

  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x_center, y_center);

  for (int i = 0; i < 360; i++)
  {
    float angle = i * M_PI / 180.0f;
    float x = x_center + radius * std::cos(angle);
    float y = y_center + radius * std::sin(angle);
    glVertex2f(x, y);
  }

  glEnd();
}

SDL_Window* Renderer::GetWindow()
{
  return window;
}
