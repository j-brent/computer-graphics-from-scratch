#pragma once

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

    inline Vector3D operator-(Position3D a, Position3D b)
    {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    inline float dot(Vector3D a, Vector3D b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

} // namespace cgfs