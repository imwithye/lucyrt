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
  static TexturePtr LoadCubemapFromInternal(
      const unsigned char* right, int right_len,    // right
      const unsigned char* left, int left_len,      // left
      const unsigned char* top, int top_len,        // top
      const unsigned char* bottom, int bottom_len,  // bottom
      const unsigned char* front, int front_len,    // front
      const unsigned char* back, int back_len       // back
  );
  static void Delete(Texture* texture);

 private:
  static std::unordered_map<std::string, TextureData> library_;
  Texture(const std::string& name, const TextureData& data);
  Texture(const std::string& name, const TextureData& right,
          const TextureData& left, const TextureData& top,
          const TextureData& bottom, const TextureData& front,
          const TextureData& back);
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
