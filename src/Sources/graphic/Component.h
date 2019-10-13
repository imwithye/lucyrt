// Copyright 2019
#pragma once

namespace lucyrt {
namespace graphic {
class Component {
 public:
  virtual bool Initialize() = 0;
  virtual void Delete() = 0;
};
}  // namespace graphic
}  // namespace lucyrt
