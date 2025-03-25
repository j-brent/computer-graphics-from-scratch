#pragma once

#include "canvas.h"
#include "color.h"
#include "extent.h"
#include "index.h"
#include "interpolation.h"
#include "mesh.h"
#include "position.h"
#include "scene.h"
#include "triangle.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <ranges>
#include <vector>

namespace cgfs
{
  namespace detail
  {
    // convert float to int using std::round()
    inline int ftoi(float f)
    {
      return static_cast<int>(std::round(f));
    }

  }
}

namespace cgfs
{
  inline cgfs::Position3D canvas_to_viewport(cgfs::Index2D C_xy, cgfs::Extent2D C_wh, cgfs::Extent2D V_wh)
  {
    const float d = 1;
    auto& [Cx, Cy] = C_xy;
    auto& [Cw, Ch] = C_wh;
    auto& [Vw, Vh] = V_wh; 
    return {Cx * float(Vw)/Cw, Cy * float(Vh)/Ch, d};
  }

  inline void render(const cgfs::Scene& scene, cgfs::Canvas& canvas, const cgfs::Extent2D& viewport)
  {
    using namespace cgfs;

    // 1. Place the camera and the viewport as desired
    const auto O = Position3D{0, 0, 0};

    // For each pixel on the canvas
    auto [Cw, Ch] = canvas.extent();
    for( int x = -Cw/2; x < Cw/2; ++x)
      for( int y = Ch/2; y > -Ch/2; --y)
      {
        // 2. Determine which square on the viewport corresponds to this pixel
        const auto V_xyz = canvas_to_viewport({x, y}, canvas.extent(), viewport);

        // 3. Determine the color seen through that square
        const auto color = scene.trace_ray({O, V_xyz - O, 1, std::numeric_limits<float>::infinity()});

        // 4. Paint the pixel that color
        canvas.putPixel({x, y}, color);
      }
  }

  inline void draw_line(Canvas& canvas, const Index2D& a, const Index2D& b, const Color& color)
  {
    for (const auto& p : interpolate(a, b))
      canvas.putPixel(p, color);
  }

  inline void draw_wireframe_triangle(Canvas& canvas, const Index2D& a, const Index2D& b, const Index2D& c, const Color& color)
  {
    draw_line(canvas, a, b, color);
    draw_line(canvas, b, c, color);
    draw_line(canvas, c, a, color);
  }

  inline void draw_filled_triangle(Canvas& canvas, const Index2D& a, const Index2D& b, const Index2D& c, const Color& color)
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

  inline void draw_filled_triangle(Canvas& canvas, const Mesh::TFace& triangle, std::ranges::random_access_range auto&& xformed, auto&& project)
  requires std::same_as<std::ranges::range_value_t<decltype(xformed)>, Position3D>
  {
    const Position3D& v0 = xformed[triangle.a];
    const Position3D& v1 = xformed[triangle.b];
    const Position3D& v2 = xformed[triangle.c];
    const Color& color = triangle.col;

    // const std::pair<Index2D, float> A = {project(v0), v0.z};
    // const std::pair<Index2D, float> B = {project(v1), v1.z};
    // const std::pair<Index2D, float> C = {project(v2), v2.z};

    // sort the vertices so that a.y <= b.y <= c.y
    auto vertices = std::array<std::pair<Index2D, float>, 3>{
      std::pair{project(v0), v0.z},
      std::pair{project(v1), v1.z},
      std::pair{project(v2), v2.z}
    };
    std::sort(vertices.begin(), vertices.end(), [](const auto& lhs, const auto& rhs){ return lhs.first.y < rhs.first.y; });
    const auto& [A, B, C] = vertices;
    const auto& [p0, z0] = A;
    const auto& [p1, z1] = B;
    const auto& [p2, z2] = C;

    // compute the x coordinates of the triangle edges
    auto x01 = interpolate(p0.y, p0.x, p1.y, p1.x);
    const auto x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
    const auto x02 = interpolate(p0.y, p0.x, p2.y, p2.x);

    auto z01 = interpolatef(p0.y, 1/z0, p1.y, 1/z1);
    const auto z12 = interpolatef(p1.y, 1/z1, p2.y, 1/z2);
    const auto z02 = interpolatef(p0.y, 1/z0, p2.y, 1/z2);

    // concatenate the short sides
    x01.pop_back();
    x01.insert(x01.end(), x12.begin(), x12.end());
    const auto& x012 = x01;

    z01.pop_back();
    z01.insert(z01.end(), z12.begin(), z12.end());
    const auto& z012 = z01;

    // determine which is left and which is right
    size_t m = x02.size() / 2;
	if (x02[m] == x012[m])
		m = std::max<size_t>(0, m - 1);
    const bool x02_left = x02[m] < x012[m];
    const auto& x_left  = x02_left ? x02  : x012;
    const auto& x_right = x02_left ? x012 : x02;
    const auto& z_left  = x02_left ? z02  : z012;
    const auto& z_right = x02_left ? z012 : z02;

    // draw the horizontal segments
    for (auto y = p0.y; y <= p2.y; ++y)
    {
      const auto& x_l = x_left.at(y-p0.y);
      const auto& x_r = x_right.at(y-p0.y);
      const auto z_segment = interpolatef(x_l, z_left.at(y-p0.y), x_r, z_right.at(y-p0.y));
      for (auto x = x_left.at(y-p0.y); x <= x_right.at(y-p0.y); ++x)
      {
        const auto z = z_segment.at(x - x_l);
        if (z > canvas.depthBuffer({x, y}))
        {
          canvas.putPixel({x, y}, color);
          canvas.depthBuffer({x, y}) = z;
        }
      }
    }
  }

  inline void draw_shaded_triangle(Canvas& canvas, const Triangle2D& t)
  {
    // for now, assume all vertices are the same color
    const auto color = t.a.col;

    // sort the vertices so that a.y <= b.y <= c.y
    auto vertices = std::array<Vertex2D, 3>{t.a, t.b, t.c};
    std::sort(vertices.begin(), vertices.end(), [](const Vertex2D& lhs, const Vertex2D& rhs){ return lhs.v.y < rhs.v.y; });
    const auto& [v0, v1, v2] = vertices;

    // compute the x coordinates of the triangle edges
    auto x01 = interpolate(v0.v.y, v0.v.x, v1.v.y, v1.v.x);
    const auto x12 = interpolate(v1.v.y, v1.v.x, v2.v.y, v2.v.x);
    const auto x02 = interpolate(v0.v.y, v0.v.x, v2.v.y, v2.v.x);

    auto h01 = interpolatef(v0.v.y, v0.intensity, v1.v.y, v1.intensity);
    const auto h12 = interpolatef(v1.v.y, v1.intensity, v2.v.y, v2.intensity);
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

  inline Index2D viewport_to_canvas(const Position2D& V_xy, const Extent2D& V_wh, const Extent2D& C_wh)
  {
    return {detail::ftoi(V_xy.x * (C_wh.width/V_wh.width)), detail::ftoi(V_xy.y * (C_wh.height/V_wh.height))};
  }

  inline Index2D project_vertex(const Position3D& v, float d, const Extent2D& V_wh, const Extent2D& C_wh)
  {
    const auto V_xy = (d / v.z) * Position2D{v.x, v.y}; // camera to viewport
    return viewport_to_canvas(V_xy, V_wh, C_wh);
  }

  inline void render_triangle(Canvas& canvas, const Mesh::TFace& triangle, std::ranges::random_access_range auto&& projected)
  requires std::same_as<std::ranges::range_value_t<decltype(projected)>, Index2D>
  {
    draw_wireframe_triangle(canvas, projected[triangle.a], projected[triangle.b], projected[triangle.c], triangle.col);
  }

  inline void render_triangle_filled(Canvas& canvas, const Mesh::TFace& triangle, std::ranges::random_access_range auto&& projected)
  requires std::same_as<std::ranges::range_value_t<decltype(projected)>, Index2D>
  {
    draw_filled_triangle(canvas, projected[triangle.a], projected[triangle.b], projected[triangle.c], triangle.col);
  }

  inline void render_triangle_filled_depth(Canvas& canvas, const Mesh::TFace& triangle, std::ranges::random_access_range auto&& xformed, auto&& project)
  requires std::same_as<std::ranges::range_value_t<decltype(xformed)>, Position3D>
  {
    draw_filled_triangle(canvas, triangle, xformed, project);
  }

  inline void render_object(Canvas& canvas, const Mesh& object, const Extent2D& V_wh, float d)
  {
    const auto project = [&](const Position3D& v){ return project_vertex(v, d, V_wh, canvas.extent());};

    for (const auto& t : object.faces)
      render_triangle(canvas, t, object.vertices | std::views::transform(project));
  }

  inline void render_instance(Canvas& canvas, auto&& instance, const Extent2D& V_wh, float d)
  {
    const auto project = [&](const Position3D& v){ return project_vertex(v, d, V_wh, canvas.extent());};

    for (const auto& t : instance.faces())
      render_triangle(canvas, t, instance.vertices() | std::views::transform(project));
  }
}
