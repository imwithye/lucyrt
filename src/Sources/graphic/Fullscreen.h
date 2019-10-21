// Copyright 2019
#pragma once

#include <memory>
#include <string>

#include <glm/vec3.hpp>

#include "GL.h"
#include "Shader.h"

namespace lucyrt {
namespace graphic {
class Fullscreen;
typedef std::shared_ptr<Fullscreen> FullscreenPtr;

class Fullscreen {
 public:
  static FullscreenPtr New(const std::string &name);
  static void Delete(Fullscreen *fullscreen);

  void Draw(Shader *program);

 private:
  std::string name_;
  GLuint vao_, vbo_;

  explicit Fullscreen(const std::string &name);
  ~Fullscreen();
};
}  // namespace graphic
}  // namespace lucyrt
