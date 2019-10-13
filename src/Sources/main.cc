// Copyright 2019
#include <iostream>
#include "graphic/graphic.h"
#include "resource/rs.h"

using lucyrt::graphic::App;
using lucyrt::graphic::Context;
using lucyrt::graphic::Fullscreen;
using lucyrt::graphic::FullscreenRef;
using lucyrt::graphic::Program;
using lucyrt::graphic::ProgramRef;

int main() {
  spdlog::set_level(spdlog::level::trace);
  App::Initialize(800, 600, "lucyrt");
  FullscreenRef fs = Fullscreen::New("postprocessing color");
  fs->Initialize();
  ProgramRef color = Program::New(
      "postprocessing color", Shaders_post_unlit_vert, Shaders_post_unlit_frag);
  color->Initialize();
  App::Run([&](Context&) { fs->Draw(color); });
  return 0;
}
