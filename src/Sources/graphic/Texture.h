// Copyright 2019
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "GL.h"

namespace lucyrt {
namespace graphic {
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class TextureData {
 public:
  TextureData();
  explicit TextureData(const std::string& filepath);
  TextureData(const unsigned char* raw_data, int len);
  TextureData(const TextureData& t);

 private:
  std::vector<GLbyte> data_;
  GLuint width_;
  GLuint height_;

  friend class Texture;
};

class Texture {
 public:
  std::string name;

  static TexturePtr LoadFromFile(const std::string& filepath);
  static TexturePtr LoadFromInternal(const unsigned char* raw_data, int len);
  static void Delete(Texture* texture);

 private:
  static std::unordered_map<std::string, TextureData> library_;
  Texture(const std::string& name, const TextureData& data);
  ~Texture();

 public:
  template <typename OStream>
  friend OStream& operator<<(OStream& os, const Texture& t) {
    return os << "Texture[" << t.name << "(id:" << t.id_ << ")]";
  }

  void Active(GLenum uint);

 private:
  GLuint id_;
};
}  // namespace graphic
}  // namespace lucyrt
