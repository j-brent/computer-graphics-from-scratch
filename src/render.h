#pragma once

#include "camera.h"
#include "canvas.h"
#include "color.h"
#include "draw.h"
#include "extent.h"
#include "index.h"
#include "cgfs_math.h"
#include "mesh.h"
#include "position.h"
#include "scene.h"

// #include <ranges>
#include <numeric>
#include <type_traits>

namespace cgfs
{
  namespace detail
  {
    auto sum(const auto& container)
    {
      using namespace std;
      const auto b = begin(container);
      const auto e = end(container);
      if (b != e)
      {
        const auto init = *b;
        return std::accumulate(next(b), e, init, [](const auto& a, const auto& b){ return a + b; });
      } else
      {
        using T = typename std::remove_cvref_t<decltype(container)>::value_type;
        return T{};
      }
    }

    inline cgfs::Position3D canvas_to_viewport(cgfs::Index2D C_xy, cgfs::Extent2D C_wh, cgfs::Extent2D V_wh)
    {
      const float d = 1;
      auto& [Cx, Cy] = C_xy;
      auto& [Cw, Ch] = C_wh;
      auto& [Vw, Vh] = V_wh; 
      return {Cx * float(Vw)/Cw, Cy * float(Vh)/Ch, d};
    }

    inline Index2D viewport_to_canvas(const Position2D& V_xy, const Extent2D& V_wh, const Extent2D& C_wh)
    {
      return {cgfs::ftoi(V_xy.x * (C_wh.width/V_wh.width)), cgfs::ftoi(V_xy.y * (C_wh.height/V_wh.height))};
    }
    
    inline Index2D project_vertex(const Position3D& v, float d, const Extent2D& V_wh, const Extent2D& C_wh)
    {
      const auto V_xy = (d / v.z) * Position2D{v.x, v.y}; // camera to viewport
      return detail::viewport_to_canvas(V_xy, V_wh, C_wh);
    }

    inline bool is_back_facing(const Vector3D& normal, const Vector3D& view)
    {
      return sp3::dot(normal, view) > 0;
    }

    inline bool is_back_facing(const Triangle3D& t)
    {
      const auto& [a, b, c, _] = t;
      const auto N = sp3::cross(b-a, c-a);
      const auto V = a - Position3D{0, 0, 0};
      return is_back_facing(N, V);
    }
    
    inline bool is_back_facing(const SuperTriangle3D& t)
    {
      const auto N = detail::sum(t.normals) / 3;
      const auto p = detail::sum(t.vertices) / 3;
      const auto V = p - Position3D{0, 0, 0};
      return is_back_facing(N, V);
    }
  }
}

namespace cgfs
{
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
        const auto V_xyz = detail::canvas_to_viewport({x, y}, canvas.extent(), viewport);

        // 3. Determine the color seen through that square
        const auto color = scene.trace_ray({O, V_xyz - O, 1, std::numeric_limits<float>::infinity()});

        // 4. Paint the pixel that color
        canvas.putPixel({x, y}, color);
      }
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

  inline void render_object(Canvas& canvas, const Mesh& object, const Extent2D& V_wh, float d)
  {
    const auto project = [&](const Position3D& v){ return detail::project_vertex(v, d, V_wh, canvas.extent());};

    for (const auto& t : object.faces)
      render_triangle(canvas, t, object.vertices | std::views::transform(project));
  }

  // an 'instance' is anything with vertices() and faces() methods
  inline void render_instance(Canvas& canvas, auto&& instance, const Extent2D& V_wh, float d)
  {
    const auto project = [&](const Position3D& v){ return detail::project_vertex(v, d, V_wh, canvas.extent());};

    for (const auto& t : instance.faces())
      render_triangle(canvas, t, instance.vertices() | std::views::transform(project));
  }

  // P is the projection operator from camera to canvas coordinates
  // M is the transformation from model to camera coordinates
  void render_model(cgfs::Canvas& canvas, auto&& model, const cgfs::Projection& P, const sp3::transform& M, const std::vector<Light>& lights = {})
  {
    const auto front_facing = [](const auto& t){ return !detail::is_back_facing(t); };

    for (const auto& t : model.triangles(M) | std::views::filter(front_facing))
      draw_filled_triangle(canvas, t, P, lights);
  }

  void render_scene(cgfs::Canvas& canvas, auto&& scene, const cgfs::Camera& camera)
  {
    // M_camera is the transformation from world to camera coordinates
    const auto M_camera = cgfs::make_camera_matrix(camera.pose());
    
    // P is the projection operator from camera to canvas coordinates
    const auto P = camera.projection(canvas.extent());

    // I.transform is the transformation from model to world coordinates
    for (const auto& I : scene.instances)
      render_model(canvas, I.model, P, M_camera * I.transform, scene.lights);
  }
}
