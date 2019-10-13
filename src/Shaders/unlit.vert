#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 LUCYRT_WORLD_TO_CAMERA;
uniform mat4 LUCYRT_LOCAL_TO_WORLD;

out vec3 Pos;
out vec3 Normal;

void main() {
  gl_Position = LUCYRT_WORLD_TO_CAMERA * LUCYRT_LOCAL_TO_WORLD *
                vec4(aPos.x, aPos.y, aPos.z, 1.0);
  Pos = vec3(LUCYRT_LOCAL_TO_WORLD * vec4(aPos.x, aPos.y, aPos.z, 1.0));
  Normal = mat3(transpose(inverse(LUCYRT_LOCAL_TO_WORLD))) * aNormal;
}
