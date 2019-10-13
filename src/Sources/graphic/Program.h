// Copyright 2019
#pragma once

#include <cstdarg>
#include <memory>
#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Component.h"
#include "GL.h"
#include "Shader.h"

namespace lucyrt {
namespace graphic {
class Program;
typedef std::shared_ptr<Program> ProgramRef;

class Program : public Component {
 public:
  static ProgramRef New(const std::string& name, const ShaderRef vert,
                        const ShaderRef frag);
  static ProgramRef New(const std::string& name, const ShaderRef vert,
                        const ShaderRef frag, const ShaderRef geom);
  static ProgramRef New(const std::string& name, const std::string& vert,
                        const std::string& frag);
  static ProgramRef New(const std::string& name, const std::string& vert,
                        const std::string& frag, const std::string& geom);

  inline std::string GetName() const { return name_; }
  inline GLuint GetId() const { return id_; }

  void Use();
  void SetBool(const std::string& name, const GLboolean value);
  void SetInt(const std::string& name, const GLint value);
  void SetFloat(const std::string& name, const GLfloat value);
  void SetVec3(const std::string& name, const glm::vec3& value);
  void SetVec4(const std::string& name, const glm::vec4& value);
  void SetMat4(const std::string& name, const glm::mat4& value);

  virtual bool Initialize();
  virtual void Delete();
  virtual ~Program();

 private:
  std::string name_;
  GLuint id_;
  ShaderRef vert_;
  ShaderRef frag_;
  ShaderRef geom_;

  Program(const std::string& name, const ShaderRef vert, const ShaderRef frag,
          const ShaderRef geom);
};
}  // namespace graphic
}  // namespace lucyrt
