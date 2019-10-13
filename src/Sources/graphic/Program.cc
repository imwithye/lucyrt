// Copyright 2019
#include "Program.h"

#include "GL.h"

using lucyrt::graphic::Program;
using lucyrt::graphic::Shader;
using lucyrt::graphic::ShaderRef;
using lucyrt::graphic::ShaderTypes;

bool Program::Initialize() {
  GLuint program_id = glCreateProgram();
  if (vert_) {
    glAttachShader(program_id, vert_->GetID());
  }
  if (frag_) {
    glAttachShader(program_id, frag_->GetID());
  }
  if (geom_) {
    glAttachShader(program_id, geom_->GetID());
  }
  glLinkProgram(program_id);
  GLint success;
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glDeleteProgram(program_id);
    return false;
  }
  id_ = program_id;
  return true;
}

void Program::Delete() { glDeleteProgram(id_); }

Program::~Program() { Delete(); }

Program::Program(const ShaderRef vert, const ShaderRef frag)
    : Program(vert, frag, nullptr) {}

Program::Program(const ShaderRef vert, const ShaderRef frag,
                 const ShaderRef geom)
    : vert_(vert), frag_(frag), geom_(geom) {}
