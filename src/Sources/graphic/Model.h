// Copyright 2019
#pragma once

#include <spdlog/fmt/ostr.h>

#include <memory>
#include <string>
#include <vector>

#include "Mesh.h"

namespace lucyrt {
namespace graphic {
class Model {
 public:
  std::vector<std::shared_ptr<Mesh>> meshes;

  explicit Model(const std::string& name, const std::string& filepath);
  ~Model();
  template <typename OStream>
  friend OStream& operator<<(OStream& os, const Model* m) {
    return os << "Model[" << m->name << "(meshes:" << m->meshes.size() << ")]";
  }

  bool PrepareToGPU();
  void RemoveFromGPU();
  void Draw();

 private:
  std::string name;
};
}  // namespace graphic
}  // namespace lucyrt
