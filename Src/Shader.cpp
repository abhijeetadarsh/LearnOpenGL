#include "Shader.hpp"
#include "VertexShader.h"
#include "FragmentShader.h"

std::string Shader::readShaderFile(const std::string &filename)
{
  // filename should be relative the working directory
  std::ifstream fileStream(filename);

  if (!fileStream.is_open())
  {
    std::cerr << "Error: Failed to open shader file: " << filename << std::endl;
    exit(2);
  }

  std::stringstream buffer;
  buffer << fileStream.rdbuf(); // Read the entire file contents into the buffer

  return buffer.str();
}

unsigned int Shader::compileShader(unsigned int type, const char *source)
{
  unsigned int id = glCreateShader(type);
  glShaderSource(id, 1, &source, nullptr);
  glCompileShader(id);

  // Error handling
  int compile_status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
  if (compile_status == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char info_log[length];
    // char *info_log = (char *)alloca(length * sizeof(char)); // another way

    glGetShaderInfoLog(id, length, &length, info_log);

    std::cerr << "Error: Failed to Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader\n"
              << info_log << std::endl;

    glDeleteShader(id);
    exit(3);
  }

  return id;
}
GLint Shader::getUniformLocation(const char *uniformVariableName) const
{
  // Check if the location is already in the cache
  if (uniformLocationCache.find(uniformVariableName) != uniformLocationCache.end())
  {
    return uniformLocationCache[uniformVariableName];
  }

  // If not in the cache, retrieve and store it
  GLint loc = glGetUniformLocation(programID, uniformVariableName);
  uniformLocationCache[uniformVariableName] = loc;
  return loc;
}

unsigned int Shader::createShader(const char *vertexShader, const char *fragmentShader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  // Error handling
  int link_status;
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);
  if (link_status == GL_FALSE)
  {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char info_log[length];
    // char *info_log = (char *)alloca(length * sizeof(char)); // another way

    glGetProgramInfoLog(program, length, &length, info_log);

    std::cerr << "Error: Failed to Link " << info_log << std::endl;

    glDeleteProgram(program);
    exit(4);
  }

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
  // std::string vertexShader = readShaderFile(vertexPath);
  // std::string fragmentShader = readShaderFile(fragmentPath);
  programID = createShader(g_VertexShaderSource, g_FragmentShaderSource);
}

Shader::~Shader()
{
  glDeleteProgram(programID);
}

void Shader::use()
{
  glUseProgram(programID);
}

void Shader::setUniform(const char *uniformVariableName, int value) const
{
  glUniform1i(getUniformLocation(uniformVariableName), value);
}

void Shader::setUniform(const char *uniformVariableName, int v1, int v2) const
{
  glUniform2i(getUniformLocation(uniformVariableName), v1, v2);
}

void Shader::setUniform(const char *uniformVariableName, int v1, int v2, int v3) const
{
  glUniform3i(getUniformLocation(uniformVariableName), v1, v2, v3);
}

void Shader::setUniform(const char *uniformVariableName, int v1, int v2, int v3, int v4) const
{
  glUniform4i(getUniformLocation(uniformVariableName), v1, v2, v3, v4);
}

void Shader::setUniform(const char *uniformVariableName, float value) const
{
  glUniform1f(getUniformLocation(uniformVariableName), value);
}

void Shader::setUniform(const char *uniformVariableName, float v1, float v2) const
{
  glUniform2f(getUniformLocation(uniformVariableName), v1, v2);
}

void Shader::setUniform(const char *uniformVariableName, float v1, float v2, float v3) const
{
  glUniform3f(getUniformLocation(uniformVariableName), v1, v2, v3);
}

void Shader::setUniform(const char *uniformVariableName, float v1, float v2, float v3, float v4) const
{
  glUniform4f(getUniformLocation(uniformVariableName), v1, v2, v3, v4);
}