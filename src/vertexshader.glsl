#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec3 outColor;

uniform mat4 transformMat4;

void main(){
   gl_Position = transformMat4 * vec4(pos, 1.0);
   outColor = color;
};