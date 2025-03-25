#pragma once

#include "color.h"
#include "extent.h"
#include "index.h"

#include <algorithm>
#include <limits>
#include <vector>

namespace cgfs
{
    class Canvas
    {
    public:

        static constexpr struct with_alpha_t {} with_alpha_channel{};

        explicit Canvas(Extent2D ext, Color bg = cgfs::Black)
        : m_extent{ext}
        , m_pixel_size_bytes{3}
        {
            m_data.resize(ext.width * ext.height * m_pixel_size_bytes);

            if (bg.r == bg.g && bg.g == bg.b)
            {
              std::ranges::fill(m_data, bg.r);
            }
            else
            {
              for (auto it = m_data.begin(); it != m_data.end(); it += m_pixel_size_bytes)
              {
                *it = bg.r;
                *(it + 1) = bg.g;
                *(it + 2) = bg.b;
              }
            }
        }
        
        explicit Canvas(with_alpha_t, Extent2D ext, Color bg = cgfs::Black, unsigned char alpha = 255)
        : m_extent{ext}
        , m_pixel_size_bytes{4}
        {
          m_data.resize(ext.width * ext.height * m_pixel_size_bytes);

          for (auto it = m_data.begin(); it != m_data.end(); it += m_pixel_size_bytes)
          {
            *it = bg.r;
            *(it + 1) = bg.g;
            *(it + 2) = bg.b;
            *(it + 3) = alpha;
          }

        }

        ~Canvas() = default;

        // Set the pixel color at the specified coordinates.
        //
        // Note that the coordinate (0, 0) is the center of the canvas.
        // Valid coordinates are in the range [-w/2, w/2] x [-h/2, h/2] where
        // w and h are the width and height of the camvas.
        //
        // Does nothing if the index is out of range
        void putPixel(Index2D xy, Color rgb);

        const Extent2D& extent() const { return m_extent; } 

        const unsigned char* data() const { return m_data.data(); }

        size_t num_bytes() const { return m_data.size() * sizeof(unsigned char); }

        // Throws std::out_of_range if the index exceeds the canvas' extent
        float& depthBuffer(Index2D xy);

    private:
        Extent2D m_extent;
        std::vector<unsigned char> m_data;
        int m_pixel_size_bytes = 3;
        std::vector<float> m_depth_buffer = std::vector<float>(size_t(m_extent.width * m_extent.height), 0);
    };

} // namespace cgfs
