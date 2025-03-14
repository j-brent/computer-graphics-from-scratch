#pragma once

#include "color.h"
#include "extent.h"
#include "index.h"

#include <algorithm>
#include <vector>

namespace cgfs
{
    class Canvas
    {
    public:
        explicit Canvas(Extent2D ext, Color bg = cgfs::Black)
        : m_extent{ext}
        {
            m_data.resize(ext.width * ext.height * 3);

            if (bg.r == bg.g && bg.g == bg.b)
            {
              std::ranges::fill(m_data, bg.r);
            }
            else
            {
              for (auto it = m_data.begin(); it != m_data.end(); it += 3)
              {
                *it = bg.r;
                *(it + 1) = bg.g;
                *(it + 2) = bg.b;
              }
            }
        }
        
        ~Canvas() = default;

        void putPixel(Index2D xy, Color rgb);

        const Extent2D& extent() const { return m_extent; } 

        const unsigned char* data() const { return m_data.data(); }

    private:
        Extent2D m_extent;
        std::vector<unsigned char> m_data;
    };

} // namespace cgfs