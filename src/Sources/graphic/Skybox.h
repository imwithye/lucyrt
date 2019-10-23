// Copyright 2019
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Context.h"
#include "GL.h"
#include "Shader.h"
#include "Texture.h"

namespace lucyrt {
namespace graphic {
class Skybox {
 public:
  void Draw(Context* ctx);

 private:
  Skybox(TexturePtr cubemap);
  ~Skybox();
  std::vector<float> skybox_vertices_;
  ShaderPtr shader_;
  TexturePtr cubemap_;
  GLuint vao_, vbo_;
};
}  // namespace graphic
}  // namespace lucyrt
