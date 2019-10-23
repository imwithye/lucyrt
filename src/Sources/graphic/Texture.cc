// Copyright 2019
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>  //NOLINT
#include <filesystem>
#include <iostream>

using lucyrt::graphic::Texture;
using lucyrt::graphic::TextureData;
using lucyrt::graphic::TexturePtr;

#define C(x) static_cast<GLbyte>(x)

TextureData::TextureData() : width_(0), height_(0) {}

TextureData::TextureData(const std::string& filepath) {
  stbi_set_flip_vertically_on_load(true);
  int width = 0, height = 0, channels = 0;
  unsigned char* data =
      stbi_load(filepath.c_str(), &width, &height, &channels, 4);
  if (!data) {
    spdlog::error("TextureData failed to load from {}", filepath);
    width_ = height_ = 2;
    data_.resize(width_ * height_ * 4);
    data_[0] = 0, data_[1] = 0, data_[2] = 0, data_[3] = C(255);
    data_[4] = C(255), data_[5] = 0, data_[6] = C(128), data_[7] = C(255);
    data_[8] = C(255), data_[9] = 0, data_[10] = C(128), data_[11] = C(255);
    data_[12] = 0, data_[13] = 0, data_[14] = 0, data_[15] = C(255);
  } else {
    width_ = width;
    height_ = height;
    data_.resize(width_ * height_ * 4);
    std::copy(data, data + data_.size(), data_.begin());
    stbi_image_free(data);
  }
  spdlog::trace("TextureData loaded from {}", filepath);
}

TextureData::TextureData(const unsigned char* raw_data, int len) {
  // stbi_set_flip_vertically_on_load(true);
  int width = 0, height = 0, channels = 0;
  unsigned char* data =
      stbi_load_from_memory(raw_data, len, &width, &height, &channels, 4);
  if (!data) {
    spdlog::error("TextureData failed to load from raw data");
    width_ = height_ = 2;
    data_.resize(width_ * height_ * 4);
    data_[0] = 0, data_[1] = 0, data_[2] = 0, data_[3] = C(255);
    data_[4] = C(255), data_[5] = 0, data_[6] = C(128), data_[7] = C(255);
    data_[8] = C(255), data_[9] = 0, data_[10] = C(128), data_[11] = C(255);
    data_[12] = 0, data_[13] = 0, data_[14] = 0, data_[15] = C(255);
  } else {
    width_ = width;
    height_ = height;
    data_.resize(width_ * height_ * 4);
    std::copy(data, data + data_.size(), data_.begin());
    stbi_image_free(data);
  }
  spdlog::trace("TextureData loaded from raw data");
}

TextureData::TextureData(const TextureData& t)
    : data_(t.data_), width_(t.width_), height_(t.height_) {}

std::unordered_map<std::string, TextureData> Texture::library_;

TexturePtr Texture::LoadFromFile(const std::string& filepath) {
  std::string filename = std::filesystem::path(filepath).filename().string();
  if (library_.find(filepath) == library_.end()) {
    library_[filepath] = TextureData(filepath);
    TexturePtr ptr(new Texture(filename, library_[filepath]), Delete);
    return ptr;
  } else {
    TexturePtr ptr(new Texture(filename, library_[filepath]), Delete);
    return ptr;
  }
}

TexturePtr Texture::LoadFromInternal(const unsigned char* raw_data, int len) {
  TexturePtr ptr(new Texture(":memory:", TextureData(raw_data, len)), Delete);
  return ptr;
}

TexturePtr Texture::LoadCubemapFromInternal(
    const unsigned char* right, int right_len,    // right
    const unsigned char* left, int left_len,      // left
    const unsigned char* top, int top_len,        // top
    const unsigned char* bottom, int bottom_len,  // bottom
    const unsigned char* front, int front_len,    // front
    const unsigned char* back, int back_len       // back
) {
  TexturePtr ptr(
      new Texture(":memory:", TextureData(right, right_len),
                  TextureData(left, left_len), TextureData(top, top_len),
                  TextureData(bottom, bottom_len),
                  TextureData(front, front_len), TextureData(back, back_len)),
      Delete);
  return ptr;
}

void Texture::Delete(Texture* texture) { delete texture; }

void Texture::Active(GLenum unit) {
  glActiveTexture(unit);
  glBindTexture(GL_TEXTURE_2D, id_);
}

Texture::Texture(const std::string& name, const TextureData& data)
    : name(name) {
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width_, data.height_, 0, GL_RGBA,
               GL_UNSIGNED_BYTE,
               reinterpret_cast<const void*>(data.data_.data()));
  glGenerateMipmap(GL_TEXTURE_2D);
  spdlog::trace("{} initialized", *this);
}

Texture::Texture(const std::string& name, const TextureData& right,
                 const TextureData& left, const TextureData& top,
                 const TextureData& bottom, const TextureData& front,
                 const TextureData& back)
    : name(name) {
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGBA, right.width_,
               right.height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               reinterpret_cast<const void*>(right.data_.data()));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGBA, left.width_,
               left.height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               reinterpret_cast<const void*>(left.data_.data()));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGBA, top.width_,
               top.height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               reinterpret_cast<const void*>(top.data_.data()));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGBA, bottom.width_,
               bottom.height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               reinterpret_cast<const void*>(bottom.data_.data()));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGBA, front.width_,
               front.height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               reinterpret_cast<const void*>(front.data_.data()));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGBA, back.width_,
               back.height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               reinterpret_cast<const void*>(back.data_.data()));
  spdlog::trace("{} initialized", *this);
}

Texture::~Texture() {
  glDeleteTextures(1, &id_);
  spdlog::trace("{} deleted", *this);
}
