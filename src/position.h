#pragma once

#include <cmath>

namespace cgfs
{
    struct Position2D
    {
        float x = 0;
        float y = 0;
    };

    struct Position3D
    {
        float x = 0;
        float y = 0;
        float z = 0;
    };

    struct Vector3D
    {
        float x = 0;
        float y = 0;
        float z = 0;
    };

    inline bool operator==(const Position3D& lhs, const Position3D& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

    inline bool operator!=(const Position3D& lhs, const Position3D& rhs)
    {
        return !(lhs == rhs);
    }

    inline bool operator==(const Vector3D& lhs, const Vector3D& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

    inline bool operator!=(const Vector3D& lhs, const Vector3D& rhs)
    {
        return !(lhs == rhs);
    }

    inline Vector3D operator-(const Vector3D& v)
    {
        return {-v.x, -v.y, -v.z};
    }

    inline Vector3D operator*(float s, const Vector3D& v)
    {
        return {s * v.x, s * v.y, s * v.z};
    }

    inline Vector3D operator/(const Vector3D& v, float s)
    {
        return {v.x / s, v.y / s, v.z / s};
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
    
    inline float dot(Vector3D a, Vector3D b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    inline Vector3D operator-(const Vector3D& a, const Vector3D& b)
    {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    inline Vector3D operator+(const Vector3D& a, const Vector3D& b)
    {
        return {a.x + b.x, a.y + b.y, a.z + b.z};
    }

    inline Vector3D operator-(Position3D a, Position3D b)
    {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    inline Position3D operator+(Position3D a, Vector3D b)
    {
        return {a.x + b.x, a.y + b.y, a.z + b.z};
    }

    class UnitVector3D
    {
        public:
            UnitVector3D(Vector3D v) : v_{normalize(v)} {}

            operator Vector3D() const { return v_; }

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