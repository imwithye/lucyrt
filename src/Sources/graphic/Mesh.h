// Copyright 2019
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Component.h"
#include "GL.h"
#include "Program.h"
#include "Texture.h"
#include "TransformationMatrix.h"

namespace lucyrt {
namespace graphic {
struct Vertex {
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 uv;
  inline bool operator==(const Vertex &rhs) const {
    return pos == rhs.pos && normal == rhs.normal && uv == rhs.uv;
  }
};

struct Material {
  glm::vec4 diffuse;
  std::vector<TextureRef> textures;
};

class Mesh;
typedef std::shared_ptr<Mesh> MeshRef;

class Mesh final : public Component {
 public:
  std::string name;
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  TransformationMatrix transform;
  Material material;

  static MeshRef New(const std::string &name);
  bool Initialize();
  void Delete();
  void Draw(ProgramRef program);
  ~Mesh();

 private:
  GLuint vao_, vbo_, ebo_;

  explicit Mesh(const std::string &name);
};
}  // namespace graphic
}  // namespace lucyrt
