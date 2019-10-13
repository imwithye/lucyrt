// Copyright 2019
#include <iostream>
#include "graphic/graphic.h"

using lucyrt::graphic::App;

int main() {
  App::Init(800, 600, "lucyrt");
  App::Run([](App& app) {});
  return 0;
}
