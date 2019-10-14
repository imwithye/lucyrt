// Copyright 2019
#include "Mesh.h"

using lucyrt::graphic::Mesh;
using lucyrt::graphic::MeshRef;

MeshRef Mesh::New(const std::string &name) {
  MeshRef mesh(new Mesh(name));
  return mesh;
}

bool Mesh::Initialize() {
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

  for (TextureRef ref : material.textures) {
    ref->Initialize();
  }
  spdlog::trace("Mesh '{}(v:{}, i:{}, {})' initialized", name, vertices.size(),
                indices.size(), vao_);
  return true;
}

void Mesh::Delete() {
  glDeleteBuffers(1, &ebo_);
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
  spdlog::trace("Mesh '{}(v:{}, i:{})' deleted", name, vertices.size(),
                indices.size());
}

void Mesh::Draw(ProgramRef program) {
  program->SetMat4("LUCYRT_LOCAL_TO_WORLD", transform.GetMatrix());
  program->SetVec4("Diffuse", material.diffuse);
  for (TextureRef ref : material.textures) {
    program->SetTexture("DiffuseTexture0", GL_TEXTURE0, ref);
  }
  program->Use();
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh() { Delete(); }

Mesh::Mesh(const std::string &name) : name(name) {}
