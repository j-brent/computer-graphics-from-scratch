#pragma once

#include "canvas.h"
#include "color.h"
#include "extent.h"
#include "index.h"
#include "interpolation.h"
#include "light.h"
#include "mesh.h"
#include "position.h"
#include "triangle.h"

#include <array>
#include <algorithm>
#include <ranges>
#include <utility>
#include <vector>

#include <iostream>

namespace cgfs
{
  namespace detail
  {
    float phong(const Index2D& xy_prime, float z_inv, float d, const Vector3D& N, const std::vector<Light>& lights)
    {
      const auto [xp, yp] = xy_prime;
      const auto p_camera = Position3D{xp/(d*z_inv), yp/(d*z_inv), 1/z_inv};
      const auto sp = SurfacePoint{p_camera, N};
      // V is the "view vector" (the direction from the surface point to the camera (in world coordinates?))
      const auto V = Position3D{0, 0, 0} - sp.pos; // vector coordinates are the same in camera and world space
      
      return std::accumulate(lights.begin(), lights.end(), 0.0f, [&](float acc, const Light& light){
        return acc + light.intensity(sp, V);
      });
    }

    // return x012, x02
    std::pair<std::vector<int>, std::vector<int>> triangle_edges(std::array<int, 3> ys, std::array<int, 3> xs)
    {
      const auto& [y0, y1, y2] = ys;
      const auto& [x0, x1, x2] = xs;

      // compute the x coordinates of the triangle edges
            auto x01 = interpolate(y0, x0, y1, x1);
      const auto x12 = interpolate(y1, x1, y2, x2);
      const auto x02 = interpolate(y0, x0, y2, x2);

      // concatenate the short sides
      x01.pop_back();
      x01.insert(x01.end(), x12.begin(), x12.end());
      
      return {x01, x02};
    }

    // return z012, z02
    template<typename T>
    std::pair<std::vector<T>, std::vector<T>> triangle_edgesf(std::array<int, 3> ys, std::array<T, 3> zs)
    {
      const auto& [y0, y1, y2] = ys;
      const auto& [z0, z1, z2] = zs;

      // compute the x coordinates of the triangle edges
            auto z01 = interpolatef<T>(y0, z0, y1, z1);
      const auto z12 = interpolatef<T>(y1, z1, y2, z2);
      const auto z02 = interpolatef<T>(y0, z0, y2, z2);

      // concatenate the short sides
      z01.pop_back();
      z01.insert(z01.end(), z12.begin(), z12.end());
      
      return {z01, z02};
    }
  }
}

namespace cgfs
{
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

  // the vertices of triangle are in camera coordinates
  // the Projection operator projects from camera (3D) coordinates to canvas (2D) coordinates
  inline void draw_filled_triangle(Canvas& canvas, const Triangle3D& triangle, const cgfs::Projection& project, const std::vector<Light>& lights = {})
  {
    const auto& [v0, v1, v2, color] = triangle;

    const float intensity = lights.empty()
    ? 1.0f
    : [&](){
        const auto sp = SurfacePoint{(v0 + v1 + v2) / 3, sp3::cross(v1-v0, v2-v0)};
        // V is the "view vector" (the direction from the surface point to the camera (in world coordinates?))
        const auto V = Position3D{0, 0, 0} - sp.pos; // vector coordinates are the same in camera and world space
        return std::accumulate(lights.begin(), lights.end(), 0.0f, [&sp, &V](float acc, const Light& light){
          return acc + light.intensity(sp, V);
        });
      } ();
      
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

    auto [x_left, x_right] = detail::triangle_edges({{p0.y, p1.y, p2.y}}, {{p0.x, p1.x, p2.x}});
    auto [z_left, z_right] = detail::triangle_edgesf<float>({{p0.y, p1.y, p2.y}}, {{1/z0, 1/z1, 1/z2}});

    // determine which is left and which is right
    size_t m = x_right.size() / 2;
    if (x_right[m] == x_left[m])
      m = std::max<size_t>(0, m - 1);

    if (x_right[m] < x_left[m])
    {
      std::swap(x_left, x_right);
      std::swap(z_left, z_right);
    }

    // draw the horizontal segments
    for (auto y = p0.y; y <= p2.y; ++y)
    {
      const size_t i = (y - p0.y);
      const auto& x_l = x_left.at(i);
      const auto& x_r = x_right.at(i);
      const auto z_segment = interpolatef(x_l, z_left.at(i), x_r, z_right.at(i));
      for (auto x = x_l; x <= x_r; ++x)
      {
        const size_t j = (x - x_l);
        const auto z = z_segment.at(j);
        if (z > canvas.depthBuffer({x, y}))
        {
          canvas.putPixel({x, y}, intensity * color);
          canvas.depthBuffer({x, y}) = z;
        }
      }
    }
  }

  // the vertices of triangle are in camera coordinates
  // the Projection operator projects from camera (3D) coordinates to canvas (2D) coordinates
  inline void draw_filled_triangle(Canvas& canvas, const SuperTriangle3D& triangle, const cgfs::Projection& project, const std::vector<Light>& lights = {})
  {
    const auto& [v0, v1, v2] = triangle.vertices;
    const auto& [n0, n1, n2] = triangle.normals;
    const auto& color = triangle.col;
    const float d = project.viewport().distance;
      
    // sort the vertices so that a.y <= b.y <= c.y
    auto vertices = std::array<std::tuple<Index2D, float, Vector3D>, 3>{{
      {project(v0), v0.z, n0},
      {project(v1), v1.z, n1},
      {project(v2), v2.z, n2}
    }};
    std::sort(vertices.begin(), vertices.end(), [](const auto& lhs, const auto& rhs){ return std::get<0>(lhs).y < std::get<0>(rhs).y; });
    const auto& [A, B, C] = vertices;
    const auto& [p0, z0, nn0] = A;
    const auto& [p1, z1, nn1] = B;
    const auto& [p2, z2, nn2] = C;

    auto [x_left, x_right] = detail::triangle_edges({{p0.y, p1.y, p2.y}}, {{p0.x, p1.x, p2.x}});
    auto [z_left, z_right] = detail::triangle_edgesf<float>({{p0.y, p1.y, p2.y}}, {{1/z0, 1/z1, 1/z2}});
    auto [nx_left, nx_right] = detail::triangle_edgesf<float>({{p0.y, p1.y, p2.y}}, {{nn0.x, nn1.x, nn2.x}});
    auto [ny_left, ny_right] = detail::triangle_edgesf<float>({{p0.y, p1.y, p2.y}}, {{nn0.y, nn1.y, nn2.y}});
    auto [nz_left, nz_right] = detail::triangle_edgesf<float>({{p0.y, p1.y, p2.y}}, {{nn0.z, nn1.z, nn2.z}});

    // determine which is left and which is right
    size_t m = x_right.size() / 2;
    if (x_right[m] == x_left[m])
      m = std::max<size_t>(0, m - 1);

    if (x_right[m] < x_left[m])
    {
      std::swap(x_left, x_right);
      std::swap(z_left, z_right);
      std::swap(nx_left, nx_right);
      std::swap(ny_left, ny_right);
      std::swap(nz_left, nz_right);
    }

    // draw the horizontal segments
    for (auto y = p0.y; y <= p2.y; ++y)
    {
      const size_t i = (y - p0.y);
      const auto& x_l = x_left.at(i);
      const auto& x_r = x_right.at(i);
      const auto z_segment = interpolatef(x_l, z_left.at(i), x_r, z_right.at(i));
      const auto nx_segment = interpolatef(x_l, nx_left.at(i), x_r, nx_right.at(i));
      const auto ny_segment = interpolatef(x_l, ny_left.at(i), x_r, ny_right.at(i));
      const auto nz_segment = interpolatef(x_l, nz_left.at(i), x_r, nz_right.at(i));
      for (auto x = x_l; x <= x_r; ++x)
      {
        const size_t j = (x - x_l);
        const auto z = z_segment.at(j);
        if (z > canvas.depthBuffer({x, y}))
        {
          const auto& nx = nx_segment.at(j);
          const auto& ny = ny_segment.at(j);
          const auto& nz = nz_segment.at(j);
          const auto intensity = detail::phong({x, y}, z, d, {nx, ny, nz}, lights);

          canvas.putPixel({x, y}, intensity * color);
          canvas.depthBuffer({x, y}) = z;
        }
      }
    }
  }


}
