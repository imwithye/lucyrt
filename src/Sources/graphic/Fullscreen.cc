// Copyright 2019
#include "Fullscreen.h"

using glm::vec3;
using lucyrt::graphic::Fullscreen;
using lucyrt::graphic::FullscreenPtr;
using lucyrt::graphic::Shader;

FullscreenPtr Fullscreen::New(const std::string &name) {
  FullscreenPtr ptr = std::shared_ptr<Fullscreen>(new Fullscreen(name), Delete);
  return ptr;
}

void Fullscreen::Delete(Fullscreen *fullscreen) { delete fullscreen; }

Fullscreen::Fullscreen(const std::string &name) : name_(name) {
  float vertices[] = {-1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                      1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,
                      1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f};
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        reinterpret_cast<void *>(2 * sizeof(float)));
  spdlog::trace("Fullscreen '{}' initialized", name_);
}

Fullscreen::~Fullscreen() {
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
  spdlog::trace("Fullscreen '{}' deleted", name_);
}

void Fullscreen::Draw(Shader *program) {
  program->Use();
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
