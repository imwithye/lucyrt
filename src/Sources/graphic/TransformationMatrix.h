// Copyright 2019
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

namespace lucyrt {
namespace graphic {
class TransformationMatrix {
 public:
  glm::mat4 matrix;

  TransformationMatrix();
  explicit TransformationMatrix(glm::mat4 matrix);
  glm::vec3 GetPos() const;
  void SetPos(glm::vec3 pos);
  glm::vec3 GetUp() const;
  glm::vec3 GetForward() const;
  void SetForward(glm::vec3 forward);
  glm::vec3 GetRight() const;
};
}  // namespace graphic
}  // namespace lucyrt
