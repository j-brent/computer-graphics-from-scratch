#include "canvas.h"

#include "extent.h"
#include "index.h"

#include <iostream>

namespace cgfs
{
  namespace 
  {
    Index2D to_screen(Index2D C_xy, Extent2D C_wh)
    {
      auto& [Cw, Ch] = C_wh;
      auto& [Cx, Cy] = C_xy; 
      return {Cw/2 + Cx, Ch/2 - Cy};
    }
  } 
}
 
namespace cgfs
  {
  void Canvas::putPixel(Index2D xy, Color rgb)
  {
    const auto& [Sx, Sy] = to_screen(xy, m_extent);
    
    if ( Sx >= m_extent.width || Sy >= m_extent.height || Sx < 0 || Sy < 0) [[unlikely]]
      return;

    int index = (Sy * m_extent.width + Sx) * 3;
    m_data[index] = rgb.r;
    m_data[index + 1] = rgb.g;
    m_data[index + 2] = rgb.b;
  }
}