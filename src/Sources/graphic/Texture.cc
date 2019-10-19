// Copyright 2019
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>  //NOLINT
#include <iostream>

using lucyrt::graphic::Texture;
using lucyrt::graphic::TexturePtr;

#define C(x) static_cast<GLbyte>(x)

std::unordered_map<std::string, TexturePtr*> Texture::library_;

TexturePtr Texture::Load(const std::string& filepath) {
  if (library_.find(filepath) == library_.end()) {
    TexturePtr ptr = std::shared_ptr<Texture>(new Texture(filepath), Delete);
    library_[filepath] = &ptr;
    return ptr;
  }
  return *library_[filepath];
}

void Texture::Delete(Texture* texture) {
  library_.erase(texture->filepath);
  delete texture;
}

void Texture::Active(GLenum unit) {
  glActiveTexture(unit);
  glBindTexture(GL_TEXTURE_2D, id_);
}

Texture::~Texture() {
  glDeleteTextures(1, &id_);
  spdlog::trace("{} deleted", this);
}

Texture::Texture(const std::string& filepath)
    : filepath(filepath), id_(0), width_(0), height_(0) {
  stbi_set_flip_vertically_on_load(true);
  int width = 0, height = 0, channels = 0;
  unsigned char* data =
      stbi_load(filepath.c_str(), &width, &height, &channels, 4);
  if (!data) {
    spdlog::error("{} failed to load from {}", this, filepath);
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
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, reinterpret_cast<void*>(data_.data()));
  glGenerateMipmap(GL_TEXTURE_2D);
  spdlog::trace("{} initialized", this);
}
