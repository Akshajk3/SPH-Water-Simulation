#include "Fluid.h"

Fluid::Fluid(int width, int height, int particle_size, float particle_mass, int screen_width, int screen_height)
  : fluidWidth(width), fluidHeight(height), particleSize(particle_size), particleMass(particle_mass), hashGrid(20), WIDTH(screen_width), HEIGHT(screen_height)
{
 for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_real_distribution<> dis(0.9, 1.0);

      double random_damp = dis(gen);

      random_damp = 0.7;

      int particle_spacing = 10;

      int particle_index = particles.size() + 1;
      Particle* particle = new Particle(100 + x * particle_spacing, 100 + y * particle_spacing, particle_size, particle_mass);
      particle->dampeningCoeff = random_damp;
      particles.push_back(particle);
    }
  }
  
  // particleSSBO = CreateParticleSSBO();

  hashGrid.Init(particles);
  
  // shaderProgram = CompileShader("../shaders/fluidSim.comp");
}

void Fluid::UpdateWindowBounds(int width, int height)
{
  WIDTH = width;
  HEIGHT = height;
}

void Fluid::CleanUp()
{
  glDeleteBuffers(1, &particleSSBO);
}

void Fluid::Update(float deltaTime)
{
  ComputeDensity();
  ComputePressure();
  ComputePressureForces();

  for (auto& particle : particles)
  {
    vec2 acceleration = particle->force / particleMass;
    acceleration.y += gravity;

    particle->vel = particle->vel + acceleration * deltaTime;
    particle->pos = particle->pos + particle->vel * deltaTime;

    particle->KeepInBounds(deltaTime, WIDTH, HEIGHT);
  }
}

// void Fluid::Update(float deltaTime)
// {
//   glUseProgram(shaderProgram);

//   glUniform1f(glGetUniformLocation(shaderProgram, "particleMass"), particleMass);
//   glUniform1f(glGetUniformLocation(shaderProgram, "smoothingLength"), smoothingLength);
//   glUniform1f(glGetUniformLocation(shaderProgram, "restDensity"), restDensity);
//   glUniform1f(glGetUniformLocation(shaderProgram, "stiffness"), stiffness);
//   glUniform1f(glGetUniformLocation(shaderProgram, "repulsionStrength"), repulsionStrength);
//   glUniform1f(glGetUniformLocation(shaderProgram, "attractionStrength"), attractionStrength);
//   glUniform1f(glGetUniformLocation(shaderProgram, "viscosity"), viscosity);
//   glUniform1f(glGetUniformLocation(shaderProgram, "deltaTime"), deltaTime);
//   glUniform1f(glGetUniformLocation(shaderProgram, "gravity"), gravity);
//   glUniform1i(glGetUniformLocation(shaderProgram, "numParticles"), particles.size());

//   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);

//   GLuint workGroupSize = (GLuint)ceil(particles.size() / 256.0f);
//   glDispatchCompute(workGroupSize, 1, 1);

//   glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
// }

void Fluid::Render(Renderer* renderer)
{
  float totalDensity = 0.0f;

  std::vector<std::pair<float, float>> positions;

  for (const auto& particle : particles)
  {
    totalDensity += particle->density;
    positions.emplace_back(particle->pos.x, particle->pos.y);
  }

  renderer->UpdateParticles(positions);
  renderer->DrawParticle(particleSize);

  avgDensity = totalDensity / particles.size();
}

void Fluid::ComputeDensity()
{
  float h = smoothingLength;
  float h2 = h * h;

  for (auto& p_i : particles)
  {
    p_i->density = 0.0f;

    std::vector<Particle*> neighbors = hashGrid.GetNeighbors(p_i);

    for (auto& p_j : neighbors)
    {
      if (p_i == p_j)
        continue;

      vec2 r_vec = p_i->pos - p_j->pos;
      float r2 = r_vec.dot(r_vec);

      if (r2 < h2)
      {
        float r = sqrt(r2);
        float W = (315.0f / (64.0f * M_PI * pow(h, 9))) * pow(h2 - r2, 3);
        p_i->density += W;
      }
    }
  }
}

void Fluid::ComputePressure()
{
  for (auto& p : particles)
  {
    p->pressure = stiffness * (p->density - restDensity);
  }
}

void Fluid::ComputePressureForces()
{
  float h = smoothingLength;
  float h2 = h * h;
  float mass = particleMass;

  for (auto& p_i : particles)
  {
    vec2 force(0.0f, 0.0f);

    std::vector<Particle*> neighbors = hashGrid.GetNeighbors(p_i);

    for (auto& p_j : neighbors)
    {
      if (p_i == p_j) continue;

      vec2 r_vec = p_i->pos - p_j->pos;
      float r2 = r_vec.dot(r_vec);

      if (r2 < h2)
      {
        float r = sqrt(r2);
        float W_grad = (-45.0f / (M_PI * std::pow(h, 6))) * std::pow(h - r, 2);

        vec2 pressureForce = r_vec.normalize() * (-mass * ((p_i->pressure + p_j->pressure) / 2.0f) * W_grad);
        force = force + pressureForce;
        
        // repulsion
        if (r < (h * 0.2f))
        {
          float dynamicRepulsion = repulsionStrength * (1.0f / (r2 + 1e-4));
          vec2 repulsion = r_vec.normalize() * dynamicRepulsion;
          force = force + repulsion;
        }
        
        // attraction 
        if (r > (h * 0.7f))
        {
          vec2 attraction = -r_vec.normalize() * (attractionStrength * 0.5f) / (r2 + 1e-4);
          force = force + attraction;
        }

        // viscosity
        vec2 velocityDifference = p_i->vel - p_j->vel;
        vec2 dampeningForce = velocityDifference * (-viscosity);
        force = force + dampeningForce; 
      }
    }
    p_i->force = p_i->force + force;
  }
}

GLuint Fluid::CreateParticleSSBO()
{
  GLuint ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particles.size() * sizeof(Particle), particles.data());

  return ssbo;
}

GLuint Fluid::CompileShader(const char* shaderPath)
{
  GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
  if (computeShader == 0) {
    std::cout << "Error: Failed to create compute shader" << std::endl;
    return 0;
  }

  std::string computeSource = GetShaderSource(shaderPath);
  if (computeSource.empty()) {
    std::cout << "Error: Shader source is empty or file could not be opened" << std::endl;
    return 0;
  }

  const char* computeCode = computeSource.c_str();
  glShaderSource(computeShader, 1, &computeCode, nullptr);
  glCompileShader(computeShader);

  int success;
  glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(computeShader, sizeof(infoLog), nullptr, infoLog);
    std::cout << "Error: Failed to compile compute shader\n" << infoLog << std::endl;
    glDeleteShader(computeShader);
    return 0;
  }

  GLuint program = glCreateProgram();
  if (program == 0) {
    std::cout << "Error: Failed to create program" << std::endl;
    glDeleteShader(computeShader);
    return 0;
  }

  glAttachShader(program, computeShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
    std::cout << "Error: Failed to link compute shader program\n" << infoLog << std::endl;
    glDeleteProgram(program);
    glDeleteShader(computeShader);
    return 0;
  }

  glDeleteShader(computeShader);

  return program;
}


std::string Fluid::GetShaderSource(const char* filename)
{
  std::ifstream file(filename);
  if (!file)
  {
    std::cout << "Error: Could not open " << filename << std::endl;
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
