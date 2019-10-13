// Copyright 2019
#include <iostream>
#include "graphic/graphic.h"
#include "resource/rs.h"

using namespace lucyrt::graphic;  // NOLINT

int main(int argc, const char** argv) {
  if (argc <= 1) {
    return -1;
  }

  spdlog::set_level(spdlog::level::trace);
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
  return 0;
}
