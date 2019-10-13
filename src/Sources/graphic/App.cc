// Copyright 2019
#include "App.h"

#include "GL.h"

using lucyrt::graphic::App;

void App::Init(int width, int height, const std::string &title) {
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

  app.width = width;
  app.height = height;
  GLFWwindow *window =
      glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(
      window,
      reinterpret_cast<GLFWframebuffersizefun>(framebufferSizeCallback));
  // glfwSetCursorPosCallback(window, Input::glfwMouseCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  app.window = window;
}

void App::Run(std::function<void(App &)> loop) {
  App &app = App::GetInstance();
  while (!glfwWindowShouldClose(reinterpret_cast<GLFWwindow *>(app.window))) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    GLdouble currentFrame = glfwGetTime();
    app.deltaTime = currentFrame - app.lastFrame;
    app.lastFrame = currentFrame;
    loop(app);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(reinterpret_cast<GLFWwindow *>(app.window));
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
}

int App::GetWidth() { return App::GetInstance().width; }

int App::GetHeight() { return App::GetInstance().height; }

float App::GetAspect() {
  return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
}

float App::GetDeltaTime() { return App::GetInstance().deltaTime; }

void App::framebufferSizeCallback(void *window, int width, int height) {
  (void)window;  // unused;
  App &app = App::GetInstance();
  app.width = width;
  app.height = height;
  glViewport(0, 0, width, height);
}
