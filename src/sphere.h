#pragma once

#include "color.h"
#include "position.h"

namespace cgfs
{
    struct Sphere
    {
        Position3D center = {0, 0, 0};
        float radius = 0;
        Color color = {0, 0, 0};
    };

    inline bool operator==(const Sphere& lhs, const Sphere& rhs)
    {
        return lhs.center.x == rhs.center.x && lhs.center.y == rhs.center.y && lhs.center.z == rhs.center.z && lhs.radius == rhs.radius && lhs.color.r == rhs.color.r && lhs.color.g == rhs.color.g && lhs.color.b == rhs.color.b;
    }

} // namespace cgfs