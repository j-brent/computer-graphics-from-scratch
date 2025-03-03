#pragma once

#include <cmath>

namespace cgfs
{
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

} // namespace cgfs