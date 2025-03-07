#include "bmp.h"
#include "canvas.h"
#include "color.h"
#include "extent.h"
#include "index.h"
#include "interpolation.h"
#include "mesh.h"
#include "position.h"
#include "scene.h"
#include "triangle.h"

#include <array>
#include <ranges>
#include <vector>

namespace cgfs
{
  namespace 
  {
    int ftoi(float f)
    {
      return static_cast<int>(std::round(f));
    }

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

    void draw_shaded_triangle(Canvas& canvas, const Triangle2D& t)
    {
      // for now, assume all vertices are the same color
      const auto color = t.a.col;

      // sort the vertices so that a.y <= b.y <= c.y
      auto vertices = std::array<Vertex2D, 3>{t.a, t.b, t.c};
      std::sort(vertices.begin(), vertices.end(), [](const Vertex2D& lhs, const Vertex2D& rhs){ return lhs.v.y < rhs.v.y; });
      const auto& [v0, v1, v2] = vertices;

      // compute the x coordinates of the triangle edges
      auto x01 = interpolate(v0.v.y, v0.v.x, v1.v.y, v1.v.x);
      auto h01 = interpolatef(v0.v.y, v0.intensity, v1.v.y, v1.intensity);
      const auto x12 = interpolate(v1.v.y, v1.v.x, v2.v.y, v2.v.x);
      const auto h12 = interpolatef(v1.v.y, v1.intensity, v2.v.y, v2.intensity);
      const auto x02 = interpolate(v0.v.y, v0.v.x, v2.v.y, v2.v.x);
      const auto h02 = interpolatef(v0.v.y, v0.intensity, v2.v.y, v2.intensity);

      // concatenate the short sides
      x01.pop_back();
      x01.insert(x01.end(), x12.begin(), x12.end());
      const auto& x012 = x01;
      h01.pop_back();
      h01.insert(h01.end(), h12.begin(), h12.end());
      const auto& h012 = h01;

      // determine which is left and which is right
      const auto m = x02.size() / 2;
      const bool x02_left = (x02[m] < x012[m]); 
      const auto& x_left = x02_left ? x02 : x012;
      const auto& h_left = x02_left ? h02 : h012;
      const auto& x_right = x02_left ? x012 : x02;
      const auto& h_right = x02_left ? h012 : h02;

      // draw the horizontal segments
      const auto& y0 = v0.v.y;
      const auto& y2 = v2.v.y;
      for (auto y = y0; y <= y2; ++y)
      {
        const auto& x_l = x_left.at(y-y0);
        const auto& x_r = x_right.at(y-y0);
        const auto h_segment = interpolatef(x_l, h_left.at(y-y0), x_r, h_right.at(y-y0));
        for (auto x = x_l; x <= x_r; ++x)
        {
          const auto shaded_color = h_segment.at(x - x_l) * color;
          canvas.putPixel({x, y}, shaded_color);
        }
      }
    }

    Index2D viewport_to_canvas(const Position2D& V_xy, const Extent2D& V_wh, const Extent2D& C_wh)
    {
      return {ftoi(V_xy.x * (C_wh.width/V_wh.width)), ftoi(V_xy.y * (C_wh.height/V_wh.height))};
    }

    Index2D project_vertex(const Position3D& v, float d, const Extent2D& V_wh, const Extent2D& C_wh)
    {
      const auto V_xy = (d / v.z) * Position2D{v.x, v.y};
      return viewport_to_canvas(V_xy, V_wh, C_wh);
    }

    void render_triangle(Canvas& canvas, const Mesh::TFace& triangle, std::ranges::random_access_range auto&& projected)
    requires std::same_as<std::ranges::range_value_t<decltype(projected)>, Index2D>
    {
      draw_wireframe_triangle(canvas, projected[triangle.a], projected[triangle.b], projected[triangle.c], triangle.col);
    }

    void render_object(Canvas& canvas, const Mesh& object, const Extent2D& V_wh, float d)
    {
      const auto project = [&](const Position3D& v){ return project_vertex(v, d, V_wh, canvas.extent());};

      for (const auto& t : object.faces)
        render_triangle(canvas, t, object.vertices | std::views::transform(project));
    }

    void render_instance(Canvas& canvas, auto&& instance, const Extent2D& V_wh, float d)
    {
      const auto project = [&](const Position3D& v){ return project_vertex(v, d, V_wh, canvas.extent());};

      for (const auto& t : instance.faces())
        render_triangle(canvas, t, instance.vertices() | std::views::transform(project));
    }

  } // namespace
}

int main()
{
  cgfs::Canvas canvas{{640, 640}};
  
  // Figure 6.5
  // cgfs::draw_line(canvas, {-200, -100}, {240, 120}, cgfs::Palette1::Orange);
  // cgfs::draw_line(canvas, {-50, -200}, {60, 240}, cgfs::Palette1::Pink);

  // Figure 7.1, 7.3
  // cgfs::draw_filled_triangle(canvas, {-200, -250}, {200, 50}, {20, 250}, cgfs::Palette1::Yellow);
  // cgfs::draw_wireframe_triangle(canvas, {-200, -250}, {200, 50}, {20, 250}, cgfs::Palette1::Purple);

  // Figure 8.1
  // cgfs::Vertex2D v0{{-200, -250}, cgfs::Palette1::Pink, 0.5f};
  // cgfs::Vertex2D v1{{200, 50}, cgfs::Palette1::Pink, 0.0f};
  // cgfs::Vertex2D v2{{20, 250}, cgfs::Palette1::Pink, 1.0f};
  // cgfs::draw_shaded_triangle(canvas, {v0, v1, v2});

  const auto viewport = cgfs::Extent2D{1, 1};
  const float d = 1;

  // Figure 9.4
  if (false)
  {
    // The four "front" vertices
    const auto vAf = cgfs::Position3D{-1-2, 1, 1};
    const auto vBf = cgfs::Position3D{ 1-2, 1, 1};
    const auto vCf = cgfs::Position3D{ 1-2, -1, 1};
    const auto vDf = cgfs::Position3D{-1-2, -1, 1};
    // The four "back" vertices
    const auto vAb = cgfs::Position3D{-1-2, 1, 2};
    const auto vBb = cgfs::Position3D{ 1-2, 1, 2};
    const auto vCb = cgfs::Position3D{ 1-2, -1, 2};
    const auto vDb = cgfs::Position3D{-1-2, -1, 2};
    
    const auto project = [&](const cgfs::Position3D& v){
      return cgfs::project_vertex(v, d, viewport, canvas.extent()); 
    };
    
    const auto front_face = std::vector<std::pair<cgfs::Index2D, cgfs::Index2D>>{
      {project(vAf), project(vBf)},
      {project(vBf), project(vCf)},
      {project(vCf), project(vDf)},
      {project(vDf), project(vAf)},
    };
    for (const auto& [a, b] : front_face)
      cgfs::draw_line(canvas, a, b, cgfs::Blue);

    const auto back_face = std::vector<std::pair<cgfs::Index2D, cgfs::Index2D>>{
      {project(vAb), project(vBb)},
    {project(vBb), project(vCb)},
    {project(vCb), project(vDb)},
    {project(vDb), project(vAb)},
  };
  for (const auto& [a, b] : back_face)
    cgfs::draw_line(canvas, a, b, cgfs::Red);

  const auto front_to_back_edges = std::vector<std::pair<cgfs::Index2D, cgfs::Index2D>>{
    {project(vAf), project(vAb)},
    {project(vBf), project(vBb)},
    {project(vCf), project(vCb)},
    {project(vDf), project(vDb)},
  };
  for (const auto& [a, b] : front_to_back_edges)
    cgfs::draw_line(canvas, a, b, cgfs::Green);
  }


  // Figure 10.2
  if(false)
  {
    const auto T = cgfs::Vector3D{-1.5, 0, 7};
    // Vertices
    auto vertices = std::vector<cgfs::Position3D> {
      cgfs::Position3D{ 1, 1, 1} + T,
      cgfs::Position3D{-1, 1, 1} + T,
      cgfs::Position3D{-1, -1, 1} + T,
      cgfs::Position3D{ 1, -1, 1} + T,
      cgfs::Position3D{ 1, 1, -1} + T,
      cgfs::Position3D{-1, 1, -1} + T,
      cgfs::Position3D{-1, -1, -1} + T,
      cgfs::Position3D{ 1, -1, -1} + T,
    };
    // Triangles
    auto faces = std::vector<cgfs::Mesh::TFace>{
      {0, 1, 2, cgfs::Red},
      {0, 2, 3, cgfs::Red},
      {4, 0, 3, cgfs::Green},
      {4, 3, 7, cgfs::Green},
      {5, 4, 7, cgfs::Blue},
      {5, 7, 6, cgfs::Blue},
      {1, 5, 6, cgfs::Yellow},
      {1, 6, 2, cgfs::Yellow},
      {4, 5, 1, cgfs::Purple},
      {4, 1, 0, cgfs::Purple},
      {2, 6, 7, cgfs::Cyan},
      {2, 7, 3, cgfs::Cyan},
    };

    const auto cube = cgfs::Mesh{std::move(vertices), std::move(faces)};
    cgfs::render_object(canvas, cube, viewport, d);
  }

  // Figure 10.3
  {
    cgfs::render_instance(canvas, cgfs::Cube{{-1.5f, 0.f, 7.f}}, viewport, d);
    cgfs::render_instance(canvas, cgfs::Cube{{1.2f, 1.0f, 6.f}}, viewport, d);
  }

  save_as_bmp(canvas, "output-rasterizer.bmp");
  
  return 0;
}