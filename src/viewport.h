#pragma once

#include "extent.h"

namespace cgfs
{
  struct Viewport
  {
    cgfs::Extent2D size = {1, 1};
    float distance = 1;
  };
}
