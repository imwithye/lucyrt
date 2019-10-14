// Copyright 2019
#include "Shader.h"

using lucyrt::graphic::Shader;

Shader::Shader(const std::string& name, const std::string& vert,
               const std::string& frag, const std::string& geom)
    : name(name), vert_(vert), frag_(frag), geom_(geom) {
  GLuint program_id = glCreateProgram();
  GLuint vert_shader_id = 0, frag_shader_id = 0, geom_shader_id = 0;
  if (vert_.size() > 0) {
    vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    const char* code = vert_.c_str();
    glShaderSource(vert_shader_id, 1, &code, NULL);
    glCompileShader(vert_shader_id);
    GLint success;
    glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(vert_shader_id, 512, NULL, infoLog);
      spdlog::error("{} vertex shader compilation faild\n{}", this, infoLog);
      glDeleteShader(vert_shader_id);
      glDeleteProgram(program_id);
      return;
    }
    glAttachShader(program_id, vert_shader_id);
  }
  if (frag_.size() > 0) {
    frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    const char* code = frag_.c_str();
    glShaderSource(frag_shader_id, 1, &code, NULL);
    glCompileShader(frag_shader_id);
    GLint success;
    glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(frag_shader_id, 512, NULL, infoLog);
      spdlog::error("{} fragment shader compilation faild\n{}", this, infoLog);
      glDeleteShader(frag_shader_id);
      glDeleteProgram(program_id);
      return;
    }
    glAttachShader(program_id, frag_shader_id);
  }
  if (geom_.size() > 0) {
    geom_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
    const char* code = geom_.c_str();
    glShaderSource(geom_shader_id, 1, &code, NULL);
    glCompileShader(geom_shader_id);
    GLint success;
    glGetShaderiv(geom_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(geom_shader_id, 512, NULL, infoLog);
      spdlog::error("{} geometry shader compilation faild\n{}", this, infoLog);
      glDeleteShader(geom_shader_id);
      glDeleteProgram(program_id);
      return;
    }
    glAttachShader(program_id, geom_shader_id);
  }
  glLinkProgram(program_id);
  GLint success;
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program_id, 512, NULL, infoLog);
    spdlog::error("{} linking faild\n{}", this, infoLog);
    glDeleteProgram(program_id);
    return;
  }

  glDeleteShader(vert_shader_id);
  glDeleteShader(frag_shader_id);
  glDeleteShader(geom_shader_id);
  id_ = program_id;
  spdlog::trace("{} initialized", this);
}

Shader::~Shader() {
  glDeleteProgram(id_);
  spdlog::trace("{} deleted", this);
}

void Shader::Use() { glUseProgram(id_); }

void Shader::SetBool(const std::string& name, const GLboolean value) {
  glUseProgram(id_);
  glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, const GLint value) {
  glUseProgram(id_);
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, const GLfloat value) {
  glUseProgram(id_);
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
  glUseProgram(id_);
  glUniform3f(glGetUniformLocation(id_, name.c_str()), value.x, value.y,
              value.z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
  glUseProgram(id_);
  glUniform4f(glGetUniformLocation(id_, name.c_str()), value.x, value.y,
              value.z, value.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
  glUseProgram(id_);
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     value_ptr(value));
}

void Shader::SetTexture(const std::string& name, const GLenum channel,
                        const std::shared_ptr<Texture> value) {
  value->Active(channel);
  SetInt(name, channel - GL_TEXTURE0);
}
