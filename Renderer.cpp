#include "Renderer.h"

Renderer::Renderer(char* windowTitle, int width, int height, const char* vertex_file, const char* fragment_file)
  :  windowName(windowTitle), width(width), height(height), vertexFile(vertex_file), fragmentFile(fragment_file)
{}

bool Renderer::Init()
{
  window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW\n";
    return false;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PROGRAM_POINT_SIZE);

  if(!InitParticleData())
    return false;  

  if (SDL_GL_MakeCurrent(window, context) < 0)
  {
      std::cout << "Failed to make OpenGL Context Current: " << SDL_GetError() << std::endl;
      return false;
  }

  const char* version = (const char*)glGetString(GL_VERSION);
  if (version)
  {
      std::cout << "OpenGL Version: " << version << std::endl;
  }
  else
  {
      std::cout << "Failed to get OpenGL Version: " << std::endl;
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

void Renderer::DrawParticle(int particle_size)
{
  GLint sizeLoc = glGetUniformLocation(shaderProgram, "pointSize");
  glUseProgram(shaderProgram);
  glUniform1f(sizeLoc, particle_size);
  glBindVertexArray(VAO);
  glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
  glBindVertexArray(0);
}

SDL_Window* Renderer::GetWindow()
{
  return window;
}

SDL_GLContext Renderer::GetContext()
{
  return context;
}

void Renderer::UpdateParticles(const std::vector<std::pair<float, float>>& positions)
{
  particleVertices.clear();

  for (const auto& [x, y] : positions)
  {
    float x_ndc = (x / 400.0f) - 1.0f;
    float y_ndc = 1.0f - (y / 300.0f);

    particleVertices.push_back(x_ndc);
    particleVertices.push_back(y_ndc);
  }

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, particleVertices.size() * sizeof(float), particleVertices.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Renderer::InitParticleData()
{
  shaderProgram = LoadShader(vertexFile, fragmentFile);
  if (shaderProgram == 0)
  {
    std::cout << "Failed to load shader program." << std::endl;
    return false;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return true;
}

GLuint Renderer::LoadShader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::string vertexSource = GetShaderSource(vertexShaderSource);
  const char* vertexCode = vertexSource.c_str();
  if (vertexCode == "")
    return 0;
  glShaderSource(vertexShader, 1, &vertexCode, nullptr);
  glCompileShader(vertexShader);

  int success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "Error: Failed to compile vertex shader\n" << infoLog << std::endl;
    return 0;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragmentSource = GetShaderSource(fragmentShaderSource);
  const char* fragmentCode = fragmentSource.c_str();
  glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "Error: Failed to compile fragment shader\n" << infoLog << std::endl;
    return 0;
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cout << "Error failed to compile shader program\n" << infoLog << std::endl;
    return 0;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

std::string Renderer::GetShaderSource(const char* filename)
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
