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
  void Reset();
  void Tick();
  void OnGUI();

 private:
  GLdouble lastX;
  GLdouble lastY;
  GLdouble yaw;
  GLdouble pitch;

  void wasd();
  void mouse();
};
}  // namespace graphic
}  // namespace lucyrt
