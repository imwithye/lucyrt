// Copyright 2019
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include "GL.h"
#include "TransformationMatrix.h"

namespace lucyrt {
namespace graphic {
class Camera {
 public:
  TransformationMatrix transform;
  GLfloat near_plane;
  GLfloat far_plane;
  GLfloat fov;
  GLfloat speed;

  Camera();
  glm::mat4 GetWorldToCamera() const;
  glm::mat4 GetSkyboxMatrix() const;
  void Reset();
  void Tick();
  void OnGUI();

 private:
  GLfloat lastX;
  GLfloat lastY;
  GLfloat yaw;
  GLfloat pitch;

  void wasd();
  void mouse();
};
}  // namespace graphic
}  // namespace lucyrt
