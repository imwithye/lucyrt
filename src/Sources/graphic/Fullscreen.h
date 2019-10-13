// Copyright 2019
#pragma once

#include <memory>
#include "Component.h"
#include "GL.h"
#include "Program.h"
#include "resource/rs.h"

namespace lucyrt {
namespace graphic {
class Fullscreen;
typedef std::shared_ptr<Fullscreen> FullscreenRef;

class Fullscreen final : public Component {
 public:
  static FullscreenRef New();

  void Draw();

  bool Initialize();
  void Delete();
  ~Fullscreen();

 private:
  GLuint vao;
  ProgramRef fullscreen;

  Fullscreen();
};
}  // namespace graphic
}  // namespace lucyrt
