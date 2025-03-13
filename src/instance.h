#pragma once

#include "mesh.h"
#include "sp3/transform.h"

namespace cgfs
{
  struct Instance
  {
    Mesh model = {};
    sp3::transform transform = {};
  };
}
