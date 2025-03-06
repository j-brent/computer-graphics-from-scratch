#pragma once

namespace cgfs
{
    struct Index2D
    {
        int x = 0;
        int y = 0;
    };

    inline Index2D operator-(const Index2D& lhs, const Index2D& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }

} // namespace cgfs