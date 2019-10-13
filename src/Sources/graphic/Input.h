// Copyright 2019
#pragma once

#include "GL.h"

namespace lucyrt {
namespace graphic {
class Input {
 private:
  static GLfloat mouseX;
  static GLfloat mouseY;

 public:
  static int GetKey(int keyCode);
  static GLfloat GetMouseX();
  static GLfloat GetMouseY();
  static int GetMouseButton(int button);

  static void glfwMouseCallback(GLFWwindow *window, GLdouble xpos,
                                GLdouble ypos);
};
}  // namespace graphic
}  // namespace lucyrt
