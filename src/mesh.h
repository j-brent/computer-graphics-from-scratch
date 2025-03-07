#pragma once

#include "color.h"
#include "position.h"

#include <vector>

namespace cgfs
{
  struct Mesh
  {
    struct TFace
    {
      size_t a;
      size_t b;
      size_t c;
      Color col;
    };
    std::vector<Position3D> vertices;
    std::vector<TFace> faces;
  };
}