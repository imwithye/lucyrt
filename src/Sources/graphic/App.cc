// Copyright 2019
#include "App.h"

#include "GL.h"

using lucyrt::graphic::App;
using lucyrt::graphic::Context;
using lucyrt::graphic::ContextRef;

ContextRef App::GetContext() { return App::GetInstance().ctx_; }

bool App::Initialize(int width, int height, const std::string &title) {
  App &app = App::GetInstance();
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE);  // uncomment this statement to fix compilation on OS X
#endif
  ContextRef ctx(new Context(width, height, title));
  if (!ctx->Initialize()) {
    return false;
  }
  GLFWwindow *window = reinterpret_cast<GLFWwindow *>(ctx->window_);
  glfwSetFramebufferSizeCallback(
      window,
      reinterpret_cast<GLFWframebuffersizefun>(framebufferSizeCallback));
  // glfwSetCursorPosCallback(window, Input::glfwMouseCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  app.ctx_ = ctx;
  return true;
}

void App::Run(std::function<void(Context &)> loop) {
  while (!GetContext()->ShouldClose()) {
    GetContext()->Loop(loop);
  }
}

void App::framebufferSizeCallback(void *window, int width, int height) {
  (void)window;  // unused;
  ContextRef ctx = App::GetContext();
  ctx->width_ = width;
  ctx->height_ = height;
  glViewport(0, 0, width, height);
}
