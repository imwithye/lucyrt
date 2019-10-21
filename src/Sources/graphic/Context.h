// Copyright 2019
#pragma once

#include <functional>
#include <memory>
#include <string>

#include "Camera.h"
#include "GL.h"
#include "Shader.h"

namespace lucyrt {
namespace graphic {
class Context;
typedef std::shared_ptr<Context> ContextRef;

class Context {
 public:
  inline GLuint GetWidth() const { return width_; }
  inline GLuint GetHeight() const { return height_; }
  inline GLfloat GetAspect() const {
    return static_cast<GLfloat>(GetWidth()) / static_cast<GLfloat>(GetHeight());
  }
  inline GLFWwindow *GetWindow() { return window_; }
  inline GLfloat GetDeltaTime() const { return deltaTime_; }
  inline Camera &GetCamera() { return camera_; }
  void PrepareShader(Shader *shader);
  bool ShouldClose() const;
  ~Context();

 protected:
  GLuint width_;
  GLuint height_;
  std::string title_;
  GLFWwindow *window_;
  GLfloat lastFrame_;
  GLfloat deltaTime_;
  Camera camera_;
  bool uiEnabled_;

  Context(GLuint width, GLuint height, const std::string &title);
  bool Initialize();
  void Delete();
  void Loop(std::function<void(Context *)> loop);

  friend class App;
};
}  // namespace graphic
}  // namespace lucyrt
