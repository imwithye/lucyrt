// Copyright 2019
#include <iostream>
#include "graphic/graphic.h"

using lucyrt::graphic::App;
using lucyrt::graphic::Context;

int main() {
  App::Initialize(800, 600, "lucyrt");
  App::Run([](Context&) {});
  return 0;
}
