// Copyright 2019
#include "Program.h"

using lucyrt::graphic::Program;
using lucyrt::graphic::ProgramRef;
using lucyrt::graphic::Shader;
using lucyrt::graphic::ShaderRef;
using lucyrt::graphic::ShaderTypes;

ProgramRef Program::New(const ShaderRef vert, const ShaderRef frag) {
  ProgramRef program(new Program(vert, frag, nullptr));
  return program;
}

ProgramRef Program::New(const ShaderRef vert, const ShaderRef frag,
                        const ShaderRef geom) {
  ProgramRef program(new Program(vert, frag, geom));
  return program;
}

ProgramRef Program::New(const std::string& vert, const std::string& frag) {
  ShaderRef v = Shader::New(vert, ShaderTypes::kVertexShader);
  ShaderRef f = Shader::New(frag, ShaderTypes::kFragmentShader);
  ProgramRef program(new Program(v, f, nullptr));
  return program;
}

ProgramRef Program::New(const std::string& vert, const std::string& frag,
                        const std::string& geom) {
  ShaderRef v = Shader::New(vert, ShaderTypes::kVertexShader);
  ShaderRef f = Shader::New(frag, ShaderTypes::kFragmentShader);
  ShaderRef g = Shader::New(geom, ShaderTypes::kGeometryShader);
  ProgramRef program(new Program(v, f, g));
  return program;
}

bool Program::Initialize() {
  GLuint program_id = glCreateProgram();
  if (vert_) {
    glAttachShader(program_id, vert_->GetId());
  }
  if (frag_) {
    glAttachShader(program_id, frag_->GetId());
  }
  if (geom_) {
    glAttachShader(program_id, geom_->GetId());
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

Program::Program(const ShaderRef vert, const ShaderRef frag,
                 const ShaderRef geom)
    : vert_(vert), frag_(frag), geom_(geom) {}
