// Copyright 2019
#pragma once

#include <string>
#include "Component.h"

namespace lucyrt {
namespace graphic {
enum ShaderTypes { kVertexShader = 0, kFragmentShader, kGeometryShader };

class Shader : public Component {
 public:
  explicit Shader(const std::string &code, ShaderTypes shader_type);
  ShaderTypes GetShaderType() const;
  virtual bool Initialize();
  virtual void Delete();
  virtual ~Shader();

 private:
  std::string code_;
  ShaderTypes shader_type_;
};
}  // namespace graphic
}  // namespace lucyrt
