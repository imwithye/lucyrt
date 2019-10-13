// Copyright 2019
#include "Input.h"
#include "App.h"

using lucyrt::graphic::Input;

GLfloat Input::mouseX = 0;
GLfloat Input::mouseY = 0;

int Input::GetKey(int keyCode) {
  return glfwGetKey(App::GetContext()->GetWindow(), keyCode);
}

GLfloat Input::GetMouseX() { return mouseX; }

GLfloat Input::GetMouseY() { return mouseY; }

int Input::GetMouseButton(int button) {
  return glfwGetMouseButton(App::GetContext()->GetWindow(), button);
}

void Input::glfwMouseCallback(GLFWwindow *window, GLdouble xpos,
                              GLdouble ypos) {
  (void)window;  // unused;
  mouseX = xpos;
  mouseY = ypos;
}
