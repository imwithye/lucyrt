// Copyright 2019
#include "Context.h"

using lucyrt::graphic::Context;

Context::~Context() { Delete(); }

bool Context::ShouldClose() const {
  return glfwWindowShouldClose(reinterpret_cast<GLFWwindow *>(window_));
}

Context::Context(GLuint width, GLuint height, const std::string &title)
    : width_(width), height_(height), title_(title) {}

bool Context::Initialize() {
  GLFWwindow *window =
      glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return false;
  }
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplOpenGL3_Init("#version 330 core");
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  window_ = window;
  return true;
}

void Context::Delete() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window_);
  glfwTerminate();
}

void Context::Loop(std::function<void(Context &)> loop) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glfwPollEvents();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  GLdouble currentFrame = glfwGetTime();
  deltaTime_ = currentFrame - lastFrame_;
  lastFrame_ = currentFrame;
  loop(*this);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window_);
}
