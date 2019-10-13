// Copyright 2019
#pragma once

#include <string>
#include <vector>
#include "Component.h"
#include "GL.h"

namespace lucyrt {
namespace graphic {
class Texture : public Component {
 public:
  virtual bool Initialize();
  virtual void Delete();
  virtual ~Texture();

 private:
  GLuint id_;
  GLuint width_;
  GLuint height_;
  GLuint channels_;
  std::vector<GLbyte> data_;

  explicit Texture(const std::string& filepath);
};
}  // namespace graphic
}  // namespace lucyrt
