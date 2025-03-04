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
        int specular = -1;    // 0: not shiny at all, 1000: very shiny
        float reflective = 0; // 0: not reflective at all, 1: a perfect mirror
    };

    static constexpr auto null_sphere = Sphere{};

    inline bool operator==(const Sphere& lhs, const Sphere& rhs)
    {
        return lhs.center == rhs.center 
            && lhs.radius == rhs.radius 
            && lhs.color == rhs.color
            && lhs.specular == rhs.specular;
    }

    inline bool operator!=(const Sphere& lhs, const Sphere& rhs)
    {
        return !(lhs == rhs);
    }

} // namespace cgfs