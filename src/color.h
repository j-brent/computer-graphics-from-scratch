#pragma once

namespace cgfs
{
    struct Color
    {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
    };

    inline Color operator*(float s, const Color& c)
    {
        // TODO: is static_cast sufficient, or should we use std::clamp first?
        return {static_cast<unsigned char>(s * c.r), static_cast<unsigned char>(s * c.g), static_cast<unsigned char>(s * c.b)};
    }

    inline Color operator*(const Color& c, float s)
    {
        return s*c;
    }

    static constexpr Color Red{255, 0, 0};
    static constexpr Color Green{0, 255, 0};
    static constexpr Color Blue{0, 0, 255};

    struct Palette1
    {
      static constexpr Color DarkGray{56, 62, 66};
      static constexpr Color Yellow{255, 178, 0};
      static constexpr Color Orange{235, 91, 0};
      static constexpr Color Pink{217, 22, 86};
      static constexpr Color Purple{100, 13, 95};
    };

} // namespace cgfs