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
typedef std::shared_ptr<Texture> TexturePtr;

class Texture {
 public:
  std::string name;

  static TexturePtr Load(const std::string& filepath);
  static void Delete(Texture* texture);

 private:
  explicit Texture(const std::string& filepath);
  ~Texture();

 public:
  template <typename OStream>
  friend OStream& operator<<(OStream& os, const Texture* t) {
    return os << "Texture[" << t->name << "(id:" << t->id_ << ")]";
  }

  void Active(GLenum uint);

 private:
  GLuint id_;
  GLuint width_;
  GLuint height_;
  std::vector<GLbyte> data_;
};
}  // namespace graphic
}  // namespace lucyrt
