const char *g_FragmentShaderSource = R"glsl(
// -------------------------------------------- Shader Start --------------------------------------------
#version 330 core
in vec4 ourColor; // the input variable from the vertex shader (same name and same type)  
in vec2 texCoord;
// uniform vec4 u_Color;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // FragColor = u_Color;
    // FragColor = vertexColor;
    FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 1.0);
}
// -------------------------------------------- Shader End --------------------------------------------
)glsl";