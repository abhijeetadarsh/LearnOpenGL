cmake_minimum_required(VERSION 3.16)
project(LearnOpenGL VERSION 1.0 LANGUAGES CXX C)

# include directories
set(GLAD_INC_DIR "Vendor/glad/include")
set(GLFW_INC_DIR "Vendor/glfw/include")
set(GLM_INC_DIR "Vendor/glm")
set(STB_INC_DIR "Vendor")
set(INC_DIR "Include")

# source directories
set(GLAD_SRC_DIR "Vendor/glad/src")
set(SRC_DIR "Src")

# shader directory
set(SHADER_DIR "Shader")

# asset directory
set(ASSET_DIR "Asset")

# Add the "Include" directory to the include paths
include_directories(${GLAD_INC_DIR})
include_directories(${GLFW_INC_DIR})
include_directories(${GLM_INC_DIR})
include_directories(${STB_INC_DIR})

include_directories(${INC_DIR})
include_directories(${SHADER_DIR})
include_directories(${ASSET_DIR})

# Add source files
file(GLOB_RECURSE SOURCE_FILES ${SRC_DIR}/*.cpp)

list(APPEND SOURCE_FILES ${GLAD_SRC_DIR}/glad.c)

add_executable(App ${SOURCE_FILES})

# Ensure we're setting this as a C++ header
set_target_properties(App PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# Find and link OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
target_link_libraries(App OpenGL::GL)

# If GLFW is not installed system-wide, you need to build it from the included source
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Vendor/glfw)
target_link_libraries(App glfw)
