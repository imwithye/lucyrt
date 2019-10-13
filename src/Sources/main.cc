// Copyright 2019
#include <iostream>
#include "./rs.h"
#include "graphic/graphic.h"

using lucyrt::graphic::App;
using lucyrt::graphic::Context;
using lucyrt::graphic::Program;
using lucyrt::graphic::ProgramRef;
using lucyrt::graphic::Shader;
using lucyrt::graphic::ShaderTypes;

int main() {
  spdlog::set_level(spdlog::level::trace);
  App::Initialize(800, 600, "lucyrt");
  ProgramRef program =
      Program::New("fullscreen", Shaders_fullscreen_vert,
                   Shaders_fullscreen_frag, Shaders_fullscreen_geom);
  program->Initialize();
  App::Run([](Context&) {});
  return 0;
}
