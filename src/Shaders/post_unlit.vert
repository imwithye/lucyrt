#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 Pos;

void main() { gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); }
