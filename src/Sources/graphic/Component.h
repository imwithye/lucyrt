// Copyright 2019
#pragma once

namespace lucyrt {
namespace graphic {
class Component {
 public:
  int GetID() const { return id_; }
  virtual bool Initialize() = 0;
  virtual void Delete() = 0;

 protected:
  int id_;
};
}  // namespace graphic
}  // namespace lucyrt
