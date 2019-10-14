// Copyright 2019
#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Component.h"
#include "GL.h"

namespace lucyrt {
namespace graphic {
class Texture;
typedef std::shared_ptr<Texture> TextureRef;

class Texture : public Component {
 public:
  static TextureRef New(const std::string& filepath);
  void Active(GLenum uint);
  virtual bool Initialize();
  virtual void Delete();
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
