// Copyright 2019
#pragma once

#include <spdlog/fmt/ostr.h>

#include <memory>
#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Context.h"
#include "GL.h"
#include "Shader.h"
#include "Texture.h"
#include "TransformationMatrix.h"

namespace lucyrt {
namespace graphic {
struct Vertex {
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 uv;
  inline bool operator==(const Vertex& rhs) const {
    return pos == rhs.pos && normal == rhs.normal && uv == rhs.uv;
  }
};

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

class Mesh {
 public:
  std::string name;

  TransformationMatrix transform;
  ShaderPtr shader;

  static MeshPtr New(const std::string& name, size_t number_of_vertices = 0,
                     size_t number_of_indices = 0);
  static void Delete(Mesh* mesh);

 private:
  explicit Mesh(const std::string& name);
  ~Mesh();

 public:
  template <typename OStream>
  friend OStream& operator<<(OStream& os, const Mesh* m) {
    return os << "Mesh[" << m->name << "(v:" << m->vertices_.size()
              << " i:" << m->indices_.size() << " vao:" << m->vao_ << ")]";
  }

  void SetVertices(const std::vector<Vertex>& vertices);
  void SetIndices(const std::vector<GLuint>& indices);
  bool PrepareToGPU();
  void RemoveFromGPU();
  void Draw(Context* ctx);

 private:
  GLuint vao_, vbo_, ebo_;
  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
};
}  // namespace graphic
}  // namespace lucyrt
