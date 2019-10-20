// Copyright 2019
#include "TransformationMatrix.h"

using glm::mat4;
using glm::vec3;
using lucyrt::graphic::TransformationMatrix;

TransformationMatrix::TransformationMatrix() : matrix(1.0) {}

TransformationMatrix::TransformationMatrix(glm::mat4 matrix) : matrix(matrix) {}

vec3 TransformationMatrix::GetPos() const {
  return vec3(matrix[0][3], matrix[1][3], matrix[2][3]);
}

void TransformationMatrix::SetPos(vec3 pos) {
  matrix[0][3] = pos[0];
  matrix[1][3] = pos[1];
  matrix[2][3] = pos[2];
}

vec3 TransformationMatrix::GetUp() const {
  return normalize(vec3(matrix[0][1], matrix[1][1], matrix[2][1]));
}

vec3 TransformationMatrix::GetForward() const {
  return -normalize(vec3(matrix[0][2], matrix[1][2], matrix[2][2]));
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
  float r = vec3(matrix[0][2], matrix[1][2], matrix[2][2])[axis] / oldf[axis];
  forward = r * normalize(forward);
  matrix[0][2] = forward[0];
  matrix[1][2] = forward[1];
  matrix[2][2] = forward[2];
}

vec3 TransformationMatrix::GetRight() const {
  return cross(GetForward(), GetUp());
}
