#version 330 core
layout(location = 0) in vec2 aPos; // the position variable has attribute position 0
layout(location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord; // the texture coordinate has attribute position 2

out vec4 ourColor; // specify a color output to the fragment shader
out vec2 texCoord;

void main() {
    gl_Position = vec4(aPos.xy /*swizzling*/, 0.0, 1.0);
    ourColor = vec4(aColor.xyz, 1.0);
    texCoord = aTexCoord;
}