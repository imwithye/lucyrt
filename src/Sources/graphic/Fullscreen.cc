// Copyright 2019
#include "Fullscreen.h"

using lucyrt::graphic::Fullscreen;
using lucyrt::graphic::FullscreenRef;
using lucyrt::graphic::Program;

FullscreenRef Fullscreen::New() {
  FullscreenRef fullscreen(new Fullscreen());
  return fullscreen;
}

void Fullscreen::Draw() {
  fullscreen->Use();
  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, 1);
}

bool Fullscreen::Initialize() {
  glGenVertexArrays(1, &vao);
  fullscreen = Program::New("fullscreen", Shaders_fullscreen_vert,
                            Shaders_fullscreen_frag, Shaders_fullscreen_geom);
  if (!fullscreen->Initialize()) {
    spdlog::error("Fullscreen initialize faild");
    glDeleteVertexArrays(1, &vao);
    return false;
  }
  spdlog::trace("Fullscreen initialized");
  return true;
}

void Fullscreen::Delete() { glDeleteVertexArrays(1, &vao); }

Fullscreen::~Fullscreen() { Delete(); }

Fullscreen::Fullscreen() : vao(0), fullscreen(nullptr) {}
