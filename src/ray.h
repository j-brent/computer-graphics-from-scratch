#pragma once

#include "position.h"

namespace cgfs
{
  struct Ray3D
  {
      Position3D origin = {0, 0, 0};
      Vector3D direction = {0, 0, 0};
      float t_min = 0;
      float t_max = 1;
  };

  static constexpr auto null_ray3d = Ray3D{};

  inline bool operator==(const Ray3D& lhs, const Ray3D& rhs)
  {
      return lhs.origin == rhs.origin && lhs.direction == rhs.direction && lhs.t_min == rhs.t_min && lhs.t_max == rhs.t_max;
  }
}