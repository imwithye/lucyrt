// Copyright 2019
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "GL.h"
#include "Mesh.h"

namespace lucyrt {
namespace graphic {
class Model;
typedef std::shared_ptr<Model> ModelPtr;

class Model {
 public:
  std::vector<MeshPtr> meshes;

  static ModelPtr LoadWithAssimp(const std::string& name,
                                 const std::string& filepath);
  static ModelPtr LoadWithVRcollab(const std::string& name,
                                   const std::string& dirpath);
  static void Delete(Model* model);

 private:
  explicit Model(const std::string& name);
  ~Model();

 public:
  template <typename OStream>
  friend OStream& operator<<(OStream& os, const Model& m) {
    return os << "Model[" << m.name << "(meshes:" << m.meshes.size() << ")]";
  }

  bool PrepareToGPU();
  void RemoveFromGPU();
  void Draw(Context* ctx);

 private:
  std::string name;
};
}  // namespace graphic
}  // namespace lucyrt
