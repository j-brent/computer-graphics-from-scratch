#pragma once

#include <algorithm>

namespace cgfs
{
    struct Color
    {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
    };

    inline bool operator==(const Color& lhs, const Color& rhs)
    {
        return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
    }

    inline Color operator*(float s, const Color& c)
    {
        const auto r = std::clamp(s * c.r, 0.f, 255.f);
        const auto g = std::clamp(s * c.g, 0.f, 255.f); 
        const auto b = std::clamp(s * c.b, 0.f, 255.f);
        return {static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b)};
    }

    inline Color operator*(const Color& c, float s)
    {
        return s*c;
    }

    inline Color operator+(const Color& lhs, const Color& rhs)
    {
        const auto r = std::clamp(lhs.r + static_cast<float>(rhs.r), 0.f, 255.f);
        const auto g = std::clamp(lhs.g + static_cast<float>(rhs.g), 0.f, 255.f); 
        const auto b = std::clamp(lhs.b + static_cast<float>(rhs.b), 0.f, 255.f);
        return {static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b)};
    }

    static constexpr Color Red{255, 0, 0};
    static constexpr Color Yellow{255, 255, 0};
    static constexpr Color Green{0, 255, 0};
    static constexpr Color Cyan{0, 255, 255};
    static constexpr Color Blue{0, 0, 255};
    static constexpr Color Purple{255, 0, 255};

    struct Palette1
    {
      static constexpr Color DarkGray{56, 62, 66};
      static constexpr Color Yellow{255, 178, 0};
      static constexpr Color Orange{235, 91, 0};
      static constexpr Color Pink{217, 22, 86};
      static constexpr Color Purple{100, 13, 95};
    };

} // namespace cgfs