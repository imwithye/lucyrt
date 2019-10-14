// Copyright 2019
#pragma once

#include <memory>
#include <string>

#include <glm/vec3.hpp>

#include "Component.h"
#include "GL.h"
#include "Shader.h"

namespace lucyrt {
namespace graphic {
class Fullscreen;
typedef std::shared_ptr<Fullscreen> FullscreenRef;

class Fullscreen : public Component {
 public:
  static FullscreenRef New(const std::string &name);

  void Draw();

  bool Initialize();
  void Delete();
  void Draw(Shader *program);
  ~Fullscreen();

 private:
  std::string name_;
  GLuint vao_, vbo_;

  explicit Fullscreen(const std::string &name);
};
}  // namespace graphic
}  // namespace lucyrt
