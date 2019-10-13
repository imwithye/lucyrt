// Copyright 2019
#pragma once

#include <cstdarg>
#include <memory>
#include <string>
#include <vector>
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
