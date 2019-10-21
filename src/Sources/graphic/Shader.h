// Copyright 2019
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "GL.h"
#include "Texture.h"

namespace lucyrt {
namespace graphic {
class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

class Shader {
 public:
  std::string name;

  static ShaderPtr Compile(const std::string& name, const std::string& vert,
                           const std::string& frag,
                           const std::string& geom = "");
  static void Delete(Shader* shader);

 private:
  Shader(const std::string& name, const std::string& vert,
         const std::string& frag, const std::string& geom = "");
  ~Shader();

 public:
  template <typename OStream>
  friend OStream& operator<<(OStream& os, const Shader& s) {
    return os << "Shader[" << s.name << "(id:" << s.id_ << ")]";
  }

  inline GLuint GetId() const { return id_; }

  glm::vec4 diffuse;
  TexturePtr diffuse_texture;

  void Use();
  void SetBool(const std::string& name, const GLboolean value);
  void SetInt(const std::string& name, const GLint value);
  void SetFloat(const std::string& name, const GLfloat value);
  void SetVec3(const std::string& name, const glm::vec3& value);
  void SetVec4(const std::string& name, const glm::vec4& value);
  void SetMat4(const std::string& name, const glm::mat4& value);
  void SetTexture(const std::string& name, const GLenum channel,
                  TexturePtr value);

 private:
  GLuint id_;
  std::string vert_;
  std::string frag_;
  std::string geom_;
};
}  // namespace graphic
}  // namespace lucyrt
