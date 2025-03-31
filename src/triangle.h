#pragma once

#include "color.h"
#include "index.h"

#include <array>

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

  struct Triangle3D
  {
    Position3D a;
    Position3D b;
    Position3D c;
    Color col;
  };

  struct SuperTriangle3D
  {
    std::array<Position3D, 3> vertices;
    std::array<UnitVector3D, 3> normals;
    Color col;
  };
}
