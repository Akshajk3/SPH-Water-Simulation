#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

class Renderer
{
  public:
    Renderer(char* windowTitle, int width,int height, const char* vertex_file, const char* fragment_file);

    SDL_Window* GetWindow();
    SDL_GLContext GetContext();

    bool Init();

    void Clear();
    void Display();
    void Destroy();

    void DrawParticle(int particle_size);

  private:
    SDL_Window* window;
    SDL_GLContext context;

    GLuint VAO, VBO;
    GLuint shaderProgram;

    std::vector<float> particleVertices;

    bool InitParticleData();

  public:
    void UpdateParticles(const std::vector<std::pair<float, float>>& positions);

  private:
    std::string GetShaderSource(const char* filename);
    GLuint LoadShader(const char* vertexShaderSource, const char* fragmentShaderSource);

    char* windowName;
    int width;
    int height;

    const char* vertexFile;
    const char* fragmentFile;
};
