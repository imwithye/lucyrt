// Copyright 2019
#include <filesystem>
#include <iostream>
#include "graphic/graphic.h"
#include "resource/rs.h"

using namespace lucyrt::graphic;  // NOLINT

int main(int argc, const char** argv) {
  spdlog::set_level(spdlog::level::trace);
  if (argc < 2) {
    spdlog::error("Usage: lucyrt <model path>");
    return -1;
  }
  App::Initialize(800, 600, "lucyrt");

  TexturePtr cubemap = Texture::LoadCubemapFromInternal(
      Textures_Skybox_right_jpg, Textures_Skybox_right_jpg_len,
      Textures_Skybox_left_jpg, Textures_Skybox_left_jpg_len,
      Textures_Skybox_top_jpg, Textures_Skybox_top_jpg_len,
      Textures_Skybox_bottom_jpg, Textures_Skybox_bottom_jpg_len,
      Textures_Skybox_front_jpg, Textures_Skybox_front_jpg_len,
      Textures_Skybox_back_jpg, Textures_Skybox_back_jpg_len);
  SkyboxPtr skybox = Skybox::New(cubemap);
  ModelPtr model;
  std::filesystem::path model_path(argv[1]);
  std::error_code error_code;
  if (std::filesystem::is_directory(model_path, error_code)) {
    model = Model::LoadWithVRcollab("model", argv[1]);
  } else if (std::filesystem::is_regular_file(model_path, error_code)) {
    model = Model::LoadWithAssimp("model", argv[1]);
  } else {
    spdlog::error("fail to load model from {}", argv[1]);
    return -1;
  }
  model->PrepareToGPU();
  App::Run([&](Context* ctx) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    model->Draw(ctx);
    skybox->Draw(ctx);
  });
  return 0;
}
