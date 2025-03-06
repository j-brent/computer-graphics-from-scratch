#include "bmp.h"
#include "canvas.h"
#include "color.h"
#include "extent.h"
#include "index.h"
#include "position.h"
#include "scene.h"

#include <iostream>

namespace cgfs
{
  namespace 
  {
    // Interpolate between two points a and b along the x-axis 
    std::vector<Index2D> interpolate_x(const Index2D& a, const Index2D& b, float slope)
    {
      if(b.x < a.x) // make sure a.x < b.x
      return interpolate_x(b, a, -slope);
      
      std::vector<Index2D> result;
      result.reserve(b.x - a.x + 1);
      
      auto y = static_cast<float>(a.y);
      const auto dy = slope;
      for (auto x = a.x; x <= b.x; ++x, y+=dy)
        result.push_back({x, static_cast<int>(std::round(y))});
      
      return result;
    }
    
    // Interpolate between two points a and b along the y-axis 
    std::vector<Index2D> interpolate_y(const Index2D& a, const Index2D& b, float slope)
    {
      if(b.y < a.y) // make sure a.y < b.y
        return interpolate_y(b, a, -slope);

      std::vector<Index2D> result;
      result.reserve(b.y - a.y + 1);
      
      auto x = static_cast<float>(a.x);
      const auto dx = 1.f / slope;
      
      for (auto y = a.y; y <= b.y; ++y, x+=dx)
        result.push_back({static_cast<int>(std::round(x)), y});

      return result;
    }

    std::vector<Index2D> interpolate(const Index2D& a, const Index2D& b)
    {
      const auto [dx, dy] = b - a;
      const auto slope = static_cast<float>(dy) / dx;

      if (std::abs(slope) < 1) // line is horizontal-ish
          return interpolate_x(a, b, slope);
      else // line is vertical-ish
          return interpolate_y(a, b, slope);
    }

    void draw_line(Canvas& canvas, const Index2D& a, const Index2D& b, const Color& color)
    {
      for (const auto& p : interpolate(a, b))
        canvas.putPixel(p, color);
    }

    void draw_line0(Canvas& canvas, const Index2D& a, const Index2D& b, const Color& color)
    {
      const auto [dx, dy] = b - a;

      if (std::abs(dx) > std::abs(dy))
      { // line is horizontal-ish
        // make sure x0 < x1
        auto& p0 = (a.x > b.x) ? b : a;
        auto& p1 = (a.x > b.x) ? a : b;

        const auto m = static_cast<float>(dy) / dx;
        auto y = static_cast<float>(p0.y);

        for (auto x = p0.x; x <= p1.x; ++x)
        {
          canvas.putPixel({x, static_cast<int>(std::round(y))}, color);
          y += m;
        }
      }
      else
      { // line is vertical-ish
        // make sure y0 < y1
        auto& p0 = (a.y > b.y) ? b : a;
        auto& p1 = (a.y > b.y) ? a : b;
        
        const auto m = static_cast<float>(dx) / dy;
        auto x = static_cast<float>(p0.x);
        
        for (auto y = p0.y; y <= p1.y; ++y)
        {
          canvas.putPixel({static_cast<int>(std::round(x)), y}, color);
          x += m;
        }
      }
    }
  }
}

int main()
{
  cgfs::Canvas canvas{{640, 640}};
  cgfs::draw_line(canvas, {-200, -100}, {240, 120}, cgfs::Palette1::Orange);
  cgfs::draw_line(canvas, {-50, -200}, {60, 240}, cgfs::Palette1::Pink);
  save_as_bmp(canvas, "output-rasterizer.bmp");
  
  return 0;
}