// Copyright 2019
#include "Program.h"

using lucyrt::graphic::Program;
using lucyrt::graphic::ProgramRef;
using lucyrt::graphic::Shader;
using lucyrt::graphic::ShaderRef;
using lucyrt::graphic::ShaderTypes;

ProgramRef Program::New(const std::string& name, const ShaderRef vert,
                        const ShaderRef frag) {
  ProgramRef program(new Program(name, vert, frag, nullptr));
  return program;
}

ProgramRef Program::New(const std::string& name, const ShaderRef vert,
                        const ShaderRef frag, const ShaderRef geom) {
  ProgramRef program(new Program(name, vert, frag, geom));
  return program;
}

ProgramRef Program::New(const std::string& name, const std::string& vert,
                        const std::string& frag) {
  ShaderRef v = Shader::New(vert, ShaderTypes::kVertexShader);
  v->Initialize();
  ShaderRef f = Shader::New(frag, ShaderTypes::kFragmentShader);
  f->Initialize();
  ProgramRef program(new Program(name, v, f, nullptr));
  return program;
}

ProgramRef Program::New(const std::string& name, const std::string& vert,
                        const std::string& frag, const std::string& geom) {
  ShaderRef v = Shader::New(vert, ShaderTypes::kVertexShader);
  v->Initialize();
  ShaderRef f = Shader::New(frag, ShaderTypes::kFragmentShader);
  f->Initialize();
  ShaderRef g = Shader::New(geom, ShaderTypes::kGeometryShader);
  g->Initialize();
  ProgramRef program(new Program(name, v, f, g));
  return program;
}

void Program::Use() { glUseProgram(id_); }

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
    char infoLog[512];
    glGetProgramInfoLog(program_id, 512, NULL, infoLog);
    spdlog::error("Program '{}' linking faild\n{}", name_, infoLog);
    glDeleteProgram(program_id);
    return false;
  }
  id_ = program_id;
  spdlog::trace("Program '{}({})' initialized", name_, id_);
  return true;
}

void Program::Delete() {
  glDeleteProgram(id_);
  spdlog::trace("Program '{}({})' deleted", name_, id_);
}

Program::~Program() { Delete(); }

Program::Program(const std::string& name, const ShaderRef vert,
                 const ShaderRef frag, const ShaderRef geom)
    : name_(name), vert_(vert), frag_(frag), geom_(geom) {}
