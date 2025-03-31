#pragma once

#include "mesh.h"
#include "sp3/transform.h"

namespace cgfs
{
  template<typename TModel = Mesh>
  struct Instance
  {
    TModel model = {};
    sp3::transform transform = {};
  };
}
