// Copyright 2019
#include "Shader.h"

#include "GL.h"

using lucyrt::graphic::Shader;
using lucyrt::graphic::ShaderRef;
using lucyrt::graphic::ShaderTypes;
using std::string;

ShaderRef Shader::New(const string& code, ShaderTypes shader_type) {
  ShaderRef shader(new Shader(code, shader_type));
  return shader;
}

ShaderTypes Shader::GetShaderType() const { return shader_type_; }

bool Shader::Initialize() {
  GLenum shader_type;
  switch (shader_type_) {
    case kVertexShader:
      shader_type = GL_VERTEX_SHADER;
      break;
    case kFragmentShader:
      shader_type = GL_FRAGMENT_SHADER;
      break;
    case kGeometryShader:
      shader_type = GL_GEOMETRY_SHADER;
      break;
    default:
      break;
  }
  GLuint shader_id = glCreateShader(shader_type);
  const char* code = code_.c_str();
  glShaderSource(shader_id, 1, &code, NULL);
  glCompileShader(shader_id);
  GLint success;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glDeleteShader(shader_id);
    return false;
  }
  id_ = shader_id;
  return true;
}

void Shader::Delete() { glDeleteShader(id_); }

Shader::~Shader() { Delete(); }

Shader::Shader(const string& code, ShaderTypes shader_type)
    : code_(code), shader_type_(shader_type) {}
