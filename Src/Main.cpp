#include <glad/glad.h>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cmath>
#include <iostream>
#include <string>

#include "AwesomeFace.h"
#include "BoardNamedMove.h"
#include "Shader.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // Calculate the new aspect ratio
  float aspectRatio = 1.0f * width / height;
  float desiredAspectRatio = 1.0f;

  // Calculate the new viewport dimensions while maintaining the aspect ratio
  int viewportWidth, viewportHeight;
  if (aspectRatio > desiredAspectRatio)
  {
    // Window is too wide
    viewportHeight = height;
    viewportWidth = height * desiredAspectRatio;
  }
  else
  {
    // Window is too tall
    viewportWidth = width;
    viewportHeight = width / desiredAspectRatio;
  }

  // Center the viewport
  int viewportX = (width - viewportWidth) / 2;
  int viewportY = (height - viewportHeight) / 2;

  glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam)
{
  std::cerr << "OpenGL Debug Message (source: " << source << ", type: " << type << ", severity: " << severity
            << "): " << message << std::endl;
}

int main(void)
{
  // -------------------------------------------- Glfw Stuff --------------------------------------------
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(600, 600, "LearnOpenGL", NULL, NULL);
  if (!window)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Set the minimum size of the window to 320 x 320
  glfwSetWindowSizeLimits(window, 320, 320, GLFW_DONT_CARE, GLFW_DONT_CARE);

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  // -------------------------------------------- Glew Stuff --------------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // -------------------------------------------- OpenGL Starts --------------------------------------------
  std::cout << "Status: Using OpenGl " << glGetString(GL_VERSION) << std::endl;
  //  Set viewport and handle resizes of the window
  glViewport(0, 0, 600, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glDebugMessageCallback(debugCallback, nullptr);
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  // -------------------------------------------- Vertices Stuff --------------------------------------------
  float rectangle_vertices[] = {
      //  2d pos /  /   rgb color  /  /tex cord/
      +0.9f, +0.9f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      +0.9f, -0.9f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.9f, -0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.9f, +0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top left
  };

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  // Core OpenGL requires that we use a VAO so it knows what to do with our vertex inputs.
  // If we fail to bind a VAO, OpenGL will most likely refuse to draw anything.
  // vertex array object (VAO), vertex buffer object (VBO), element buffer object (EBO)
  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(/* count -> */ 1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // we could've also specified the stride as 0 to let OpenGL determine the stride
  // (this only works when values are tightly packed)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(2 * sizeof(float)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(5 * sizeof(float)));

  // --------------------------------------------Textures Stuff --------------------------------------------
  int widthImg, heightImg, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *img_data;

  // ---------------------- Texture 1 ----------------------
  unsigned int texture1;
  glGenTextures(1, &texture1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  img_data = stbi_load_from_memory(BoardNamedMove_png, BoardNamedMove_png_len, &widthImg, &heightImg, &nrChannels, 0);
  // unsigned char *img_data = stbi_load("Asset/BoardNamedMove.png", &widthImg, &heightImg, &nrChannels, 0);
  if (img_data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
  }
  else
  {
    std::cerr << "Failed to load texture1" << std::endl;
    exit(-1);
  }
  stbi_image_free(img_data);
  img_data = nullptr;

  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // ---------------------- Texture 2 ----------------------
  unsigned int texture2;
  glGenTextures(1, &texture2);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  img_data = stbi_load_from_memory(AwesomeFace_png, AwesomeFace_png_len, &widthImg, &heightImg, &nrChannels, 0);
  // img_data = stbi_load("Asset/awesomeface.png", &widthImg, &heightImg, &nrChannels, 0);
  if (img_data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
  }
  else
  {
    std::cerr << "Failed to load texture2" << std::endl;
    exit(-1);
  }
  stbi_image_free(img_data);
  img_data = nullptr;

  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // -------------------------------------------- Shader Stuff --------------------------------------------

  Shader shaderForRectangle("Shader/VertexShader.vert", "Shader/FragmentShader.frag");
  shaderForRectangle.use();
  shaderForRectangle.setUniform("texture1", 0);
  shaderForRectangle.setUniform("texture2", 1);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Note that finding the uniform location does not require you to use the shader program first,
  // but updating a uniform does require you to first use the program (by calling glUseProgram),
  // int loc = glGetUniformLocation(shader_program, "u_Color");

  // int nrAttributes;
  // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  // std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

  // -------------------------------------------- Loop --------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    /* Process Input */
    processInput(window);

    /* Render here */
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // float timeValue = glfwGetTime();
    // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    // glUniform4f(loc, 0.4f, 0.4f, 0.4f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  // -------------------------------------------- Cleaning Stuff --------------------------------------------
  glfwTerminate();
  return 0;
}
