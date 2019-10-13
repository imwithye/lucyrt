// Copyright 2019
#pragma once

#include <memory>
#include <string>
#include "Component.h"

namespace lucyrt {
namespace graphic {
enum ShaderTypes { kVertexShader = 0, kFragmentShader, kGeometryShader };

class Shader;
typedef std::shared_ptr<Shader> ShaderRef;

class Shader : public Component {
 public:
  ShaderRef New(const std::string &code, ShaderTypes shader_type);
  ShaderTypes GetShaderType() const;
  virtual bool Initialize();
  virtual void Delete();
  virtual ~Shader();

 private:
  std::string code_;
  ShaderTypes shader_type_;

  Shader(const std::string &code, ShaderTypes shader_type);
};

}  // namespace graphic
}  // namespace lucyrt
