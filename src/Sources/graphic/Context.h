// Copyright 2019
#pragma once

#include <functional>
#include <memory>
#include <string>
#include "Component.h"

namespace lucyrt {
namespace graphic {
class Context;
typedef std::shared_ptr<Context> ContextRef;

class Context : public Component {
 public:
  inline int GetWidth() const { return width_; }
  inline int GetHeight() const { return height_; }
  inline float GetAspect() const {
    return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
  }
  inline float GetDeltaTime() const { return deltaTime_; }
  bool ShouldClose() const;
  virtual ~Context();

 protected:
  int width_;
  int height_;
  std::string title_;
  void *window_;
  float lastFrame_;
  float deltaTime_;

  Context(int width, int height, const std::string &title);
  virtual bool Initialize();
  virtual void Delete();
  void Loop(std::function<void(Context &)> loop);

  friend class App;
};
}  // namespace graphic
}  // namespace lucyrt
