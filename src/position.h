#pragma once

#include "sp3/point.h"
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

    inline float length(const Vector3D& v)
    {
        return std::hypot(v.x, v.y, v.z);
    }

    inline Vector3D normalize(const Vector3D& v)
    {
        float len = length(v);
        return {v.x / len, v.y / len, v.z / len};
    }

    class UnitVector3D
    {
    public:
      UnitVector3D() = default;
      ~UnitVector3D() = default;

      UnitVector3D(const UnitVector3D&) = default;
      UnitVector3D& operator=(const UnitVector3D&) = default;

      UnitVector3D(UnitVector3D&&) = default;
      UnitVector3D& operator=(UnitVector3D&&) = default;


      UnitVector3D(Vector3D v) : v_{normalize(v)} {}
      UnitVector3D(float x, float y, float z) : UnitVector3D{Vector3D{x, y, z}} {}

      operator Vector3D() { return v_; }
      operator const Vector3D() const { return v_; }

    private:
        Vector3D v_;
    };

    struct SurfacePoint
    {
      Position3D pos = {};
      UnitVector3D normal = {{0, 0, 0}};
      int specularity = -1;
    };

} // namespace cgfs
