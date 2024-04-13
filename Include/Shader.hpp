#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class Shader
{
private:
  unsigned int programID;
  // Cache for uniform locations
  mutable std::unordered_map<std::string, int> uniformLocationCache;

  std::string readShaderFile(const std::string &filename);
  unsigned int compileShader(unsigned int type, const char *source);
  unsigned int createShader(const char *vertexShaderSource, const char *fragmentShaderSource);
  GLint getUniformLocation(const char *uniformVariableName) const;

public:
  // the program ID

  // constructor reads and builds the shader
  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();
  // use/activate the shader
  void use();
  // utility uniform functions
  void setUniform(const char *uniformVariableName, int) const;
  void setUniform(const char *uniformVariableName, int, int) const;
  void setUniform(const char *uniformVariableName, int, int, int) const;
  void setUniform(const char *uniformVariableName, int, int, int, int) const;

  void setUniform(const char *uniformVariableName, float) const;
  void setUniform(const char *uniformVariableName, float, float) const;
  void setUniform(const char *uniformVariableName, float, float, float) const;
  void setUniform(const char *uniformVariableName, float, float, float, float) const;
};