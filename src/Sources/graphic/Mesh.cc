// Copyright 2019
#include "Mesh.h"

#include "App.h"
#include "resource/rs.h"  //NOLINT

using lucyrt::graphic::Mesh;
using lucyrt::graphic::MeshPtr;

MeshPtr Mesh::New(const std::string &name, size_t number_of_vertices,
                  size_t number_of_indices) {
  MeshPtr ptr = std::shared_ptr<Mesh>(new Mesh(name), Delete);
  ptr->vertices_.resize(number_of_vertices);
  ptr->indices_.resize(number_of_indices);
  return ptr;
}

void Mesh::Delete(Mesh *mesh) { delete mesh; }

Mesh::Mesh(const std::string &name) : name(name), vao_(0), vbo_(0), ebo_(0) {
  shader = Shader::Compile("shader", Shaders_blinn_phong_vert,
                           Shaders_blinn_phong_frag);
}

Mesh::~Mesh() {
  RemoveFromGPU();
  spdlog::trace("{} deleted", this);
}

void Mesh::SetVertices(const std::vector<Vertex> &vertices) {
  vertices_ = vertices;
}

void Mesh::SetSubmeshCount(GLuint count) { indices_.resize(count); }

void Mesh::SetIndices(GLuint idx, const std::vector<GLuint> &indices) {
  indices_[idx] = indices;
}

bool Mesh::PrepareToGPU() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
               vertices_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  std::vector<GLuint> indices;
  for (size_t i = 0; i < indices_.size(); i++) {
    const std::vector<GLuint> &submesh = indices_[i];
    indices.insert(indices.end(), submesh.begin(), submesh.end());
  }
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, pos)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, uv)));
  glEnableVertexAttribArray(2);

  spdlog::trace("{} loaded to GPU", this, vao_);
  return true;
}

void Mesh::RemoveFromGPU() {
  glDeleteBuffers(1, &ebo_);
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
  vao_ = vbo_ = ebo_ = 0;
  spdlog::trace("{} removed from GPU", this);
}

void Mesh::Draw(Context *ctx) {
  ctx->PrepareShader(shader.get());
  shader->SetMat4("LUCYRT_LOCAL_TO_WORLD", transform.matrix);
  shader->SetVec4("Diffuse", shader->diffuse);
  if (shader->diffuse_texture)
    shader->SetTexture("DiffuseTexture0", GL_TEXTURE0, shader->diffuse_texture);
  shader->Use();
  glBindVertexArray(vao_);
  size_t offset = 0;
  for (size_t i = 0; i < indices_.size(); i++) {
    const std::vector<GLuint> &indices = indices_[i];
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT,
                   reinterpret_cast<char *>(NULL) + offset);
    offset += sizeof(GLuint) * indices.size();
  }
  glBindTexture(GL_TEXTURE_2D, 0);
}
