#pragma once

namespace cgfs
{
    struct Color
    {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
    };

    struct Palette1
    {
      static constexpr Color DarkGray{56, 62, 66};
      static constexpr Color Yellow{255, 178, 0};
      static constexpr Color Orange{235, 91, 0};
      static constexpr Color Pink{217, 22, 86};
      static constexpr Color Purple{100, 13, 95};
    };

} // namespace cgfs