#include "bmp.h"
#include "canvas.h"
#include "color.h"
#include "extent.h"
#include "index.h"
#include "interpolation.h"
#include "position.h"
#include "scene.h"

#include <array>

namespace cgfs
{
  namespace 
  {
    void draw_line(Canvas& canvas, const Index2D& a, const Index2D& b, const Color& color)
    {
      for (const auto& p : interpolate(a, b))
        canvas.putPixel(p, color);
    }

    void draw_wireframe_triangle(Canvas& canvas, const Index2D& a, const Index2D& b, const Index2D& c, const Color& color)
    {
      draw_line(canvas, a, b, color);
      draw_line(canvas, b, c, color);
      draw_line(canvas, c, a, color);
    }

    void draw_filled_triangle(Canvas& canvas, const Index2D& a, const Index2D& b, const Index2D& c, const Color& color)
    {
      // sort the vertices so that a.y <= b.y <= c.y
      auto vertices = std::array<Index2D, 3>{a, b, c};
      std::sort(vertices.begin(), vertices.end(), [](const Index2D& lhs, const Index2D& rhs){ return lhs.y < rhs.y; });
      const auto& [p0, p1, p2] = vertices;

      // compute the x coordinates of the triangle edges
      auto x01 = interpolate(p0.y, p0.x, p1.y, p1.x);
      const auto x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
      const auto x02 = interpolate(p0.y, p0.x, p2.y, p2.x);

      // concatenate the short sides
      x01.pop_back();
      x01.insert(x01.end(), x12.begin(), x12.end());
      const auto& x012 = x01;

      // determine which is left and which is right
      const auto m = x02.size() / 2;
      const auto& x_left = (x02[m] < x012[m]) ? x02 : x012;
      const auto& x_right = (x02[m] < x012[m]) ? x012 : x02;

      // draw the horizontal segments
      for (auto y = p0.y; y <= p2.y; ++y)
        for (auto x = x_left.at(y-p0.y); x <= x_right.at(y-p0.y); ++x)
          canvas.putPixel({x, y}, color);
    }

  } // namespace
}

int main()
{
  cgfs::Canvas canvas{{640, 640}};
  
  // Figure 6.5
  // cgfs::draw_line(canvas, {-200, -100}, {240, 120}, cgfs::Palette1::Orange);
  // cgfs::draw_line(canvas, {-50, -200}, {60, 240}, cgfs::Palette1::Pink);

  // Figure 7.1
  cgfs::draw_filled_triangle(canvas, {-200, -250}, {200, 50}, {20, 250}, cgfs::Palette1::Yellow);
  cgfs::draw_wireframe_triangle(canvas, {-200, -250}, {200, 50}, {20, 250}, cgfs::Palette1::Purple);

  save_as_bmp(canvas, "output-rasterizer.bmp");
  
  return 0;
}