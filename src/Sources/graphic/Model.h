// Copyright 2019
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "Mesh.h"
#include "Program.h"

namespace lucyrt {
namespace graphic {
class Model;
typedef std::shared_ptr<Model> ModelRef;

class Model : public Component {
 public:
  std::vector<MeshRef> meshes;

  static ModelRef New(const std::string &name, const std::string &filepath);
  bool Initialize();
  void Delete();
  void Draw(ProgramRef program);
  ~Model();

 private:
  std::string name_;
  explicit Model(const std::string &name);
};
}  // namespace graphic
}  // namespace lucyrt
