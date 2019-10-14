// Copyright 2019
#pragma once

#include <spdlog/fmt/ostr.h>

#include <memory>
#include <string>
#include <vector>

#include "GL.h"

namespace lucyrt {
namespace graphic {
class Texture;
typedef std::shared_ptr<Texture> TextureRef;

class Texture {
 public:
  std::string name;

  static TextureRef New(const std::string& filepath);

  template <typename OStream>
  friend OStream& operator<<(OStream& os, const Texture* t) {
    return os << "Texture[" << t->name << "(" << t->id_ << ")]";
  }

  void Active(GLenum uint);
  virtual ~Texture();

 private:
  GLuint id_;
  GLuint width_;
  GLuint height_;
  std::vector<GLbyte> data_;

  explicit Texture(const std::string& filepath);
};
}  // namespace graphic
}  // namespace lucyrt
