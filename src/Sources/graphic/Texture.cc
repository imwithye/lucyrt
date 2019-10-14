// Copyright 2019
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>  //NOLINT
#include <iostream>

using lucyrt::graphic::Texture;
using lucyrt::graphic::TextureRef;

TextureRef Texture::New(const std::string& filepath) {
  TextureRef ref(new Texture(filepath));
  return ref;
}

void Texture::Active(GLenum unit) {
  glActiveTexture(unit);
  glBindTexture(GL_TEXTURE_2D, id_);
}

bool Texture::Initialize() {
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, reinterpret_cast<void*>(data_.data()));
  glGenerateMipmap(GL_TEXTURE_2D);
  spdlog::info("Texture '(w: {}, h: {})' initialized", width_, height_);
  return true;
}

void Texture::Delete() { glDeleteTextures(1, &id_); }

Texture::~Texture() { Delete(); }

Texture::Texture(const std::string& filepath) : id_(0), width_(0), height_(0) {
  stbi_set_flip_vertically_on_load(true);
  int width = 0, height = 0, channels = 0;
  unsigned char* data =
      stbi_load(filepath.c_str(), &width, &height, &channels, 4);
  if (!data) {
    spdlog::error("Texture failed to load from {}", filepath);
    return;
  }
  width_ = width;
  height_ = height;
  data_.resize(width_ * height_ * 4);
  std::copy(data, data + data_.size(), data_.begin());
  stbi_image_free(data);
}
