#pragma once

#include "color.h"
#include "index.h"

namespace cgfs
{

  struct Vertex2D
  {
    Index2D v = {};
    Color col = {};
    float intensity = 1.f; // 0: black, 1: full color
  };

  struct Triangle2D
  {
    Vertex2D a;
    Vertex2D b;
    Vertex2D c;
  };
}