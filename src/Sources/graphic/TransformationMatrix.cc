// Copyright 2019
#include "TransformationMatrix.h"

using glm::mat4;
using glm::vec3;
using lucyrt::graphic::TransformationMatrix;

TransformationMatrix::TransformationMatrix() : matrix_(1.0) {}

mat4 TransformationMatrix::GetMatrix() const { return matrix_; }

vec3 TransformationMatrix::GetPos() const {
  return vec3(matrix_[0][3], matrix_[1][3], matrix_[2][3]);
}

void TransformationMatrix::SetPos(vec3 pos) {
  matrix_[0][3] = pos[0];
  matrix_[1][3] = pos[1];
  matrix_[2][3] = pos[2];
}

vec3 TransformationMatrix::GetUp() const {
  return normalize(vec3(matrix_[0][1], matrix_[1][1], matrix_[2][1]));
}

vec3 TransformationMatrix::GetForward() const {
  return -normalize(vec3(matrix_[0][2], matrix_[1][2], matrix_[2][2]));
}

void TransformationMatrix::SetForward(vec3 forward) {
  vec3 oldf = GetForward();
  int axis = -1;
  if (oldf[0] != 0) {
    axis = 0;
  } else if (oldf[1] != 0) {
    axis = 1;
  } else if (oldf[2] != 0) {
    axis = 2;
  }
  float r =
      vec3(matrix_[0][2], matrix_[1][2], matrix_[2][2])[axis] / oldf[axis];
  forward = r * normalize(forward);
  matrix_[0][2] = forward[0];
  matrix_[1][2] = forward[1];
  matrix_[2][2] = forward[2];
}

vec3 TransformationMatrix::GetRight() const {
  return cross(GetForward(), GetUp());
}
