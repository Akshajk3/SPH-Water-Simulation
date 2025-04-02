#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "Renderer.h"
#include "Fluid.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;

int Init_SDL2()
{
  if (SDL_Init(SDL_INIT_VIDEO) > 0)
  {
    std::cout << "Failed to init SDL2: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  return 0;
}

void Init_ImGui(Renderer* renderer)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(renderer->GetWindow(), renderer->GetContext());
  ImGui_ImplOpenGL3_Init("#version 330 core");
}

void SaveConfig(Fluid* fluid, const char* filepath)
{
  std::ofstream file(filepath);
  if (file.is_open())
  {
    file << fluid->particleSize << "\n";
    file << fluid->particleMass << "\n";
    file << fluid->gravity << "\n";
    file << fluid->restDensity << "\n";
    file << fluid->stiffness << "\n";
    file << fluid->repulsionStrength << "\n";
    file << fluid->attractionStrength << "\n";
    file << fluid->smoothingLength << "\n";

    file.close();
  }
}

void LoadConfig(Fluid* fluid, const char* filepath)
{
  std::ifstream file(filepath);
  if (file.is_open())
  {
    file >> fluid->particleSize;
    file >> fluid->particleMass;
    file >> fluid->gravity;
    file >> fluid->restDensity;
    file >> fluid->stiffness;
    file >> fluid->repulsionStrength;
    file >> fluid->attractionStrength;
    file >> fluid->smoothingLength;

    file.close();
  }
}

int main(int argc, char** argv)
{
  bool running = true;

  if (Init_SDL2() != 0)
  {
    running = false;
  }

  Renderer renderer("Flip Water Sim", WIDTH, HEIGHT, "../shaders/vert.glsl", "../shaders/frag.glsl");
  
  if (!renderer.Init())
  {
    running = false;
  }

  Fluid fluid(30, 30, 3, 10.0, WIDTH, HEIGHT);

  Init_ImGui(&renderer);

  SDL_Event e;

  Uint32 frameStart;
  float deltaTime = 0.0;

  LoadConfig(&fluid, "../config.txt");

  while (running)
  {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&e))
    {
      ImGui_ImplSDL2_ProcessEvent(&e);
      if (e.type == SDL_QUIT)
        running = false;
      
      if (e.type == SDL_KEYDOWN)
      {
        if (e.key.keysym.sym == SDLK_ESCAPE)
          running = false; 
      }

      if (e.type == SDL_WINDOWEVENT)
      {
        if (e.window.event == SDL_WINDOWEVENT_RESIZED)
        {
          int newWidth = e.window.data1;
          int newHeight = e.window.data2;
          std::cout << "Window Resized to: " << newWidth << "x" << newHeight << std::endl;

          SDL_GL_SetSwapInterval(1);
          glViewport(0, 0, newWidth, newHeight);

          fluid.UpdateWindowBounds(newWidth, newHeight);
        }
      }
    }
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();

    ImGui::Begin("Simulation Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

    // Particle Size
    ImGui::SliderInt("Particle Size", &fluid.particleSize, 1.0f, 10.f);
    ImGui::SameLine();
    ImGui::InputInt("##ParticleSizeInput", &fluid.particleSize, 1.0f);

    // Particle Mass
    ImGui::SliderFloat("Particle Mass", &fluid.particleMass, 0.0f, 1000.0f);
    ImGui::SameLine();
    ImGui::InputFloat("##ParticleMassInput", &fluid.particleMass, 0.1f, 1.0f, "%.3f");

    // Gravity
    ImGui::SliderFloat("Gravity", &fluid.gravity, -1000.0f, 1000.0f);
    ImGui::SameLine();
    ImGui::InputFloat("##GravityInput", &fluid.gravity, 0.1f, 1.0f, "%.3f");

    // Rest Density
    ImGui::SliderFloat("Rest Density", &fluid.restDensity, 0.0f, 200.0f);
    ImGui::SameLine();
    ImGui::InputFloat("##RestDensityInput", &fluid.restDensity, 0.1f, 1.0f, "%.3f");

    // Stiffness
    ImGui::SliderFloat("Stiffness", &fluid.stiffness, 0.0f, 1000.0f);
    ImGui::SameLine();
    ImGui::InputFloat("##StiffnessInput", &fluid.stiffness, 0.1f, 1.0f, "%.3f");

    // Repulsion Strength
    ImGui::SliderFloat("Repulsion Strength", &fluid.repulsionStrength, 0.0f, 100.0f);
    ImGui::SameLine();
    ImGui::InputFloat("##RepulsionStrengthInput", &fluid.repulsionStrength, 0.1f, 1.0f, "%.3f");

    // Attraction Strength
    ImGui::SliderFloat("Attraction Strength", &fluid.attractionStrength, 0.0f, 100.0f);
    ImGui::SameLine();
    ImGui::InputFloat("##AttractionStrengthInput", &fluid.attractionStrength, 0.1f, 1.0f, "%.3f");

    // Smoothing Length
    ImGui::SliderFloat("Smoothing Length", &fluid.smoothingLength, 0.0f, 100.0f);
    ImGui::SameLine();
    ImGui::InputFloat("##SmoothingLengthInput", &fluid.smoothingLength, 0.1f, 1.0f, "%.3f");

    // Average Density
    std::string densityStr = "Average Density: " + std::to_string(fluid.avgDensity);
    ImGui::Text("%s", densityStr.c_str());

    // Save Parameters
    if (ImGui::Button("Save Config"))
    {
      SaveConfig(&fluid, "../config.txt");
    }

    // Load Parameters
    if (ImGui::Button("Load Config"))
    {
      LoadConfig(&fluid, "../config.txt");
    }
    
        
    ImGui::End();


    renderer.Clear();
    fluid.Update(deltaTime);
    fluid.Render(&renderer);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    renderer.Display();

    deltaTime = (SDL_GetTicks() - frameStart) / 1000.0f;

    int frameTime = SDL_GetTicks() - frameStart;

    int frameRate = 1000 / frameTime;
    std::string windowName = "Flip Water Sim FPS: " + std::to_string(frameRate);
    SDL_SetWindowTitle(renderer.GetWindow(), windowName.c_str());

    if (1000 / FPS > frameTime) 
    {
        SDL_Delay(static_cast<Uint32>((1000 / FPS) - frameTime));
    }
  }
  

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  fluid.CleanUp();
  renderer.Destroy();
  SDL_Quit();
  return 0;
}
