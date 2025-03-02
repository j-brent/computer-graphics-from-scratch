#pragma once

#include "color.h"
#include "extent.h"
#include "index.h"

namespace cgfs
{
    class Canvas
    {
    public:
        explicit Canvas(Extent2D ext) : m_extent{ext}
        {
            m_data = new unsigned char[ext.width * ext.height * 3];
        }
        
        ~Canvas(){ delete[] m_data; }

        void putPixel(Index2D xy, Color rgb);

        const Extent2D& extent() const { return m_extent; } 

        const unsigned char* data() const { return m_data; }

    private:
        Extent2D m_extent;
        unsigned char* m_data;
    };

} // namespace cgfs