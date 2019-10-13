// Copyright 2019
#include <iostream>
#include "graphic/graphic.h"
#include "resource/rs.h"

using lucyrt::graphic::App;
using lucyrt::graphic::Context;
using lucyrt::graphic::Model;
using lucyrt::graphic::ModelRef;
using lucyrt::graphic::Program;
using lucyrt::graphic::ProgramRef;

int main() {
  spdlog::set_level(spdlog::level::trace);
  App::Initialize(800, 600, "lucyrt");
  ModelRef model = Model::New("model", "../example/cube.obj");
  model->Initialize();
  ProgramRef unlit =
      Program::New("unlit", Shaders_unlit_vert, Shaders_unlit_frag);
  unlit->Initialize();
  App::Run([&](Context&) { model->Draw(unlit); });
  return 0;
}
