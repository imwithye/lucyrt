// Copyright 2019
#include <iostream>
#include "graphic/graphic.h"
#include "resource/rs.h"

using namespace lucyrt::graphic;  // NOLINT

void fullscreen(int argc, const char** argv) {
  (void)argc;
  (void)argv;
  App::Initialize(800, 600, "lucyrt");
  ProgramRef pp_unlit =
      Program::New("post-processing unlit", Shaders_postprocessing_unlit_vert,
                   Shaders_postprocessing_unlit_frag);
  pp_unlit->Initialize();
  FullscreenRef fs = Fullscreen::New("texture");
  fs->Initialize();
  TextureRef tex = Texture::New("/home/yiwei/lucyrt/examples/tiles.png");
  tex->Initialize();
  App::Run([&](Context&) {
    pp_unlit->SetTexture("tex", GL_TEXTURE0, tex);
    fs->Draw(pp_unlit);
  });
}

void model(int argc, const char** argv) {
  if (argc <= 1) {
    return;
  }
  App::Initialize(800, 600, "lucyrt");
  ModelRef model = Model::New("model", argv[1]);
  model->Initialize();
  ProgramRef blinn_phong = Program::New("blinn-phong", Shaders_blinn_phong_vert,
                                        Shaders_blinn_phong_frag);
  blinn_phong->Initialize();
  App::Run([&](Context& ctx) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Camera c = ctx.GetCamera();
    blinn_phong->SetMat4("LUCYRT_WORLD_TO_CAMERA", c.GetWorldToCamera());
    blinn_phong->SetVec3("LUCYRT_CAMERA_POS", c.transform.GetPos());
    model->Draw(blinn_phong);
  });
}

int main(int argc, const char** argv) {
  spdlog::set_level(spdlog::level::trace);
  // fullscreen(argc, argv);
  model(argc, argv);
  return 0;
}
