// Copyright 2019
#include "Context.h"
#include "Camera.h"
#include "GL.h"

using lucyrt::graphic::Context;

Context::~Context() { Delete(); }

void Context::PrepareShader(Shader *shader) {
  shader->SetMat4("LUCYRT_WORLD_TO_CAMERA", camera_.GetWorldToCamera());
  shader->SetVec3("LUCYRT_CAMERA_POS", camera_.transform.GetPos());
}

bool Context::ShouldClose() const {
  return glfwWindowShouldClose(reinterpret_cast<GLFWwindow *>(window_));
}

Context::Context(GLuint width, GLuint height, const std::string &title)
    : width_(width),
      height_(height),
      title_(title),
      window_(nullptr),
      lastFrame_(0),
      deltaTime_(0),
      uiEnabled_(true) {}

bool Context::Initialize() {
  GLFWwindow *window =
      glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
  if (!window) {
    spdlog::error("Context '{}' failed to create window", title_);
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("Context '{}' failed to initialize GLAD", title_);
    return false;
  }
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplOpenGL3_Init("#version 330 core");
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  window_ = window;
  spdlog::trace("Context '{}({} x {})' initialized", title_, width_, height_);
  return true;
}

void Context::Delete() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window_);
  glfwTerminate();
  spdlog::trace("Context '{}({} x {})' deleted", title_, width_, height_);
}

void Context::Loop(std::function<void(Context *)> loop) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glfwPollEvents();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  GLdouble currentFrame = glfwGetTime();
  deltaTime_ = currentFrame - lastFrame_;
  lastFrame_ = currentFrame;
  camera_.Tick();

  if (uiEnabled_) {
    ImGui::Begin(
        title_.c_str(), &uiEnabled_,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("Stats");
    ImGui::Text("%.1f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::Separator();
    camera_.OnGUI();
  }
  loop(this);
  if (uiEnabled_) {
    ImGui::End();
  }
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window_);
}
