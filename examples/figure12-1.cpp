#include "bmp.h"
#include "camera.h"
#include "canvas.h"
#include "extent.h"
#include "instance.h"
#include "mesh.h"
#include "render.h"

#include "sp3/angle.h"
#include "sp3/axes.h"
#include "sp3/transform.h"

#include <ranges>

namespace
{
  struct Viewport
  {
    cgfs::Extent2D size = {1, 1};
    float distance = 1;
  };

  struct MeshScene
  {
    std::vector<cgfs::Instance<cgfs::Mesh>> instances = {};
  };

  class Projection
  {
  public:
    explicit Projection(cgfs::Extent2D canvas_dimensions, cgfs::Extent2D viewport = {1, 1}, float d = 1)
    : m_canvas_dimensions{std::move(canvas_dimensions)}
    , m_viewport{std::move(viewport)}
    , m_d{d}
    {}
    
    // project the 3D pt p (in camera space) onto the 2D canvas (integer coords)
    // via the 2D viewport (real coords)
    cgfs::Index2D operator()(const cgfs::Position3D& p_camera) const
    {
      const auto& [vx, vy, vz] = p_camera;
      const auto& [Cw, Ch] = m_canvas_dimensions;
      const auto& [Vw, Vh] = m_viewport;
      return {
        static_cast<int>(std::round((Cw / Vw) * (m_d / vz) * vx)),
        static_cast<int>(std::round((Ch / Vh) * (m_d / vz) * vy))
      };
    }

  private:
    cgfs::Extent2D m_canvas_dimensions = {0, 0}; 
    cgfs::Extent2D m_viewport = {1, 1};
    float m_d = 1;
  };


  // M is the transformation from model to camera coordinates
  void render_model(const cgfs::Mesh& model, cgfs::Canvas& canvas, const Viewport& vp, const sp3::transform& M)
  {
    // p is the projection operator from camera to canvas coordinates
    const auto p = Projection(canvas.extent(), vp.size, vp.distance);
    const auto project = [&](const cgfs::Position3D& v) -> cgfs::Index2D { return p(M(v)); };

    for (const auto& t : model.faces)
      render_triangle_filled(canvas, t, model.vertices | std::views::transform(project));
  }

  void render_scene(const MeshScene& scene, cgfs::Canvas& canvas, const cgfs::Camera& camera, const Viewport& vp)
  {
    const auto M_camera = cgfs::make_camera_matrix(camera.pose());

    for (const auto& I : scene.instances)
      render_model(I.model, canvas, vp, M_camera * I.transform);
  }
}

int main()
{
  const auto scene = MeshScene{std::vector<cgfs::Instance<cgfs::Mesh>>{
    {cgfs::wireframe_cube(), sp3::transform{{-1.5f, 0.f, 7.f}, {}}},
    {cgfs::wireframe_cube(), sp3::transform{{1.2f, 1.0f, 6.f}, {}}},
  }};
  cgfs::Canvas canvas{{640, 640}};
  const auto camera = cgfs::Camera{ {{0, -1, 0}, {sp3::xhat, sp3::angle{-sp3::pi/12}}}} ;
  const auto viewport = Viewport{};

  render_scene(scene, canvas, camera, viewport);

  save_as_bmp(canvas, "figure12-1.bmp");
}
