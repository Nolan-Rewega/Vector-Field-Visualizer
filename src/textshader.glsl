#version 330 core
layout (location = 2) in vec3 textPos;
layout (location = 3) in vec2 vertex;
out vec2 TexCoords;

uniform mat4 testMat4;

void main()
{
    gl_Position = testMat4 * vec4(textPos, 1.0);
    TexCoords = vertex;
}  