// Copyright 2019
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/vec3.hpp>

#include "Component.h"
#include "GL.h"
#include "Program.h"

namespace lucyrt {
namespace graphic {
struct Vertex {
  glm::vec3 pos;
  glm::vec3 normal;
  inline bool operator==(const Vertex &rhs) const {
    return pos == rhs.pos && normal == rhs.normal;
  }
};

struct Material {};

class Mesh;
typedef std::shared_ptr<Mesh> MeshRef;

class Mesh final : public Component {
 public:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  static MeshRef New(const std::string &name);
  bool Initialize();
  void Delete();
  void Draw(ProgramRef program);
  ~Mesh();

 private:
  std::string name_;
  GLuint vao_, vbo_, ebo_;

  explicit Mesh(const std::string &name);
};
}  // namespace graphic
}  // namespace lucyrt
