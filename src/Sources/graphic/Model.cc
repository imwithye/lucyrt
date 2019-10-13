// Copyright 2019
#include "Model.h"

using lucyrt::graphic::Model;
using lucyrt::graphic::ModelRef;
using lucyrt::graphic::ProgramRef;

ModelRef Model::New(const std::string &name, const std::string &filepath) {
  (void)filepath;
  ModelRef ref(new Model(name));
  return ref;
}

bool Model::Initialize() {
  for (MeshRef mesh : meshes) {
    mesh->Initialize();
  }
  spdlog::trace("Model '{}(meshes: {})' initialized", name_, meshes.size());
  return true;
}

void Model::Delete() {
  for (MeshRef mesh : meshes) {
    mesh->Delete();
  }
  spdlog::trace("Model '{}(meshes: {})' deleted", name_, meshes.size());
}

void Model::Draw(ProgramRef program) {
  for (MeshRef mesh : meshes) {
    mesh->Draw(program);
  }
}

Model::~Model() {}

Model::Model(const std::string &name) : name_(name) {}
