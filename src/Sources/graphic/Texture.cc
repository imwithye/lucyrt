// Copyright 2019
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>  //NOLINT
#include <iostream>

using lucyrt::graphic::Texture;

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
  return true;
}

void Texture::Delete() {}

Texture::~Texture() {}

Texture::Texture(const std::string& filepath) {
  stbi_set_flip_vertically_on_load(true);
  int width, height, channels;
  unsigned char* data =
      stbi_load(filepath.c_str(), &width, &height, &channels, 0);
  width_ = width;
  height_ = height;
  channels_ = channels;
  data_.resize(width * height_ * channels_);
  std::copy(data, data + data_.size(), data_.data());
  stbi_image_free(data);
}
