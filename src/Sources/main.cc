// Copyright 2019
#include <iostream>
#include "graphic/graphic.h"
#include "resource/rs.h"

using namespace lucyrt::graphic;  // NOLINT

void fullscreen(int argc, const char** argv) {
  (void)argc;
  (void)argv;
  App::Initialize(800, 600, "lucyrt");
  ShaderPtr pp_unlit = Shader::Compile(
      "post-processing unlit",
      reinterpret_cast<const char*>(Shaders_postprocessing_unlit_vert),
      reinterpret_cast<const char*>(Shaders_postprocessing_unlit_frag));
  FullscreenPtr fs = Fullscreen::New("texture");
  TexturePtr tex =
      Texture::LoadFromFile("/home/yiwei/lucyrt/examples/tiles.png");
  App::Run([&](Context*) {
    pp_unlit->SetTexture("tex", GL_TEXTURE0, tex);
    fs->Draw(pp_unlit.get());
  });
}

void model(int argc, const char** argv) {
  if (argc <= 1) {
    return;
  }
  App::Initialize(800, 600, "lucyrt");
  ModelPtr model = Model::LoadWithVRcollab("model", argv[1]);
  model->PrepareToGPU();
  App::Run([&](Context* ctx) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    model->Draw(ctx);
  });
}

int main(int argc, const char** argv) {
  spdlog::set_level(spdlog::level::trace);
  // fullscreen(argc, argv);
  model(argc, argv);
  return 0;
}
