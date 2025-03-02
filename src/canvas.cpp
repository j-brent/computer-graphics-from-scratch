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
    auto S_xy = to_screen(xy, m_extent);

    const auto& [x, y] = S_xy;
    int index = (y * m_extent.width + x) * 3;
    // std::cout << x << ", " << y <<  " -- " << index << std::endl;
    m_data[index] = rgb.r;
    m_data[index + 1] = rgb.g;
    m_data[index + 2] = rgb.b;
  }
}