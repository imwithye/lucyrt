// Copyright 2019
#pragma once

#include <functional>
#include <string>
#include "Context.h"

namespace lucyrt {
namespace graphic {
class App {
 public:
  static App &GetInstance() {
    static App instance;
    return instance;
  }
  App(App const &) = delete;
  void operator=(App const &) = delete;

  static ContextRef GetContext();
  static bool Initialize(GLuint width, GLuint height, const std::string &title);
  static void Run(std::function<void(Context &)> loop);

 private:
  ContextRef ctx_;

  App() : ctx_(nullptr) {}
  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
};
}  // namespace graphic
}  // namespace lucyrt
