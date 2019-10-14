// Copyright 2019
#include "Mesh.h"

#include "App.h"
#include "resource/rs.h"

using lucyrt::graphic::Mesh;
using lucyrt::graphic::MeshRef;

MeshRef Mesh::New(const std::string &name) {
  MeshRef mesh(new Mesh(name));
  return mesh;
}

Mesh::Mesh(const std::string &name) : name(name), vao_(0), vbo_(0), ebo_(0) {
  shader = std::make_shared<Shader>("shader", Shaders_blinn_phong_vert,
                                    Shaders_blinn_phong_frag);
}

Mesh::~Mesh() {
  RemoveFromGPU();
  spdlog::trace("{} deleted", this);
}

bool Mesh::PrepareToGPU() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
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

void Mesh::Draw() {
  shader->SetMat4("LUCYRT_WORLD_TO_CAMERA",
                  App::GetContext()->GetCamera().GetWorldToCamera());
  shader->SetVec3("LUCYRT_CAMERA_POS",
                  App::GetContext()->GetCamera().transform.GetPos());
  shader->SetMat4("LUCYRT_LOCAL_TO_WORLD", transform.GetMatrix());
  shader->SetVec4("Diffuse", shader->diffuse);
  if (shader->diffuse_texture)
    shader->SetTexture("DiffuseTexture0", GL_TEXTURE0, shader->diffuse_texture);
  shader->Use();
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
