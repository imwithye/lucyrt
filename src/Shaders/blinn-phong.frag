#version 330 core

uniform vec3 LUCYRT_CAMERA_POS;

uniform vec3 LUCYRT_AMBIENT_COLOR = vec3(0.5, 0.5, 0.5);
uniform vec3 LUCYRT_LIGHT_DIR = vec3(-1, -1, 0);
uniform vec3 LUCYRT_LIGHT_COLOR = vec3(0.8, 0.8, 0.8);

uniform float LUCYRT_GAMMA = 2.0;

uniform vec4 Diffuse = vec4(0, 0, 0, 1);

in vec3 Pos;
in vec3 Normal;
out vec4 FragColor;

void main() {
  vec3 normal = normalize(Normal);
  vec3 lightDir = normalize(-LUCYRT_LIGHT_DIR);
  vec3 viewDir = normalize(LUCYRT_CAMERA_POS - Pos);
  vec3 halfwayDir = normalize(lightDir + viewDir);

  vec3 ambient = LUCYRT_AMBIENT_COLOR;
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * LUCYRT_LIGHT_COLOR;
  float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
  vec3 specular = 0.5 * spec * LUCYRT_LIGHT_COLOR;

  FragColor = vec4((diffuse + ambient + specular), 1) * Diffuse;
  FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / LUCYRT_GAMMA));
}