// Copyright 2019
#pragma once

#include <functional>
#include <iostream>
#include <string>

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

  static void Init(int width, int height, const std::string &title);
  static void Run(std::function<void(App &)> loop);
  static int GetWidth();
  static int GetHeight();
  static float GetAspect();
  static float GetDeltaTime();

 private:
  int width = 800;
  int height = 600;
  void *window = nullptr;
  float deltaTime = 0;
  float lastFrame = 0;

  App() {}
  static void framebufferSizeCallback(void *window, int width, int height);
};
}  // namespace graphic
}  // namespace lucyrt
