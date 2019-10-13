// Copyright 2019
#include <iostream>
#include "graphic/graphic.h"

using lucyrt::graphic::App;
using lucyrt::graphic::Context;
using lucyrt::graphic::Fullscreen;
using lucyrt::graphic::FullscreenRef;

int main() {
  spdlog::set_level(spdlog::level::trace);
  App::Initialize(800, 600, "lucyrt");
  FullscreenRef fs = Fullscreen::New();
  fs->Initialize();
  App::Run([&](Context&) { fs->Draw(); });
  return 0;
}
