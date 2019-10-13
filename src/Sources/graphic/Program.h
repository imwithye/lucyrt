// Copyright 2019
#pragma once

#include <cstdarg>
#include <memory>
#include <string>
#include <vector>
#include "Component.h"
#include "Shader.h"

namespace lucyrt {
namespace graphic {
class Program;
typedef std::shared_ptr<Program> ProgramRef;

class Program : public Component {
 public:
  ProgramRef New(const ShaderRef vert, const ShaderRef frag);
  ProgramRef New(const ShaderRef vert, const ShaderRef frag,
                 const ShaderRef geom);
  virtual bool Initialize();
  virtual void Delete();
  virtual ~Program();

 private:
  ShaderRef vert_;
  ShaderRef frag_;
  ShaderRef geom_;

  Program(const ShaderRef vert, const ShaderRef frag);
  Program(const ShaderRef vert, const ShaderRef frag, const ShaderRef geom);
};
}  // namespace graphic
}  // namespace lucyrt
