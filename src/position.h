#pragma once

#include "sp3/point.h"
#include "sp3/unit_vector.h"
#include "sp3/vector.h"
#include "sp3/vector_point_functions.h"

#include <cmath>

namespace cgfs
{
    struct Position2D
    {
        float x = 0;
        float y = 0;
    };

    inline Position2D operator*(float s, const Position2D& p)
    {
      return {s*p.x, s*p.y};
    }
    
    using Position3D = sp3::point;
    using Vector3D = sp3::vector;

    inline Position3D operator+(const Position3D& lhs, const Position3D& rhs)
    {
      return {lhs.x + rhs.x, lhs.y + rhs.y, + lhs.z + rhs.z};
    }

    inline Position3D operator/(const Position3D& p, float s)
    {
      return {p.x/s, p.y/s, + p.z/s};
    }

    struct SurfacePoint
    {
      Position3D pos = {};
      sp3::unit_vector normal = {{0, 0, 0}};
      int specularity = -1;
    };

} // namespace cgfs
