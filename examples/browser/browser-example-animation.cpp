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

#include <emscripten.h>

#include <format>
#include <iostream>
#include <ranges>
#include <stdio.h>

// https://gist.github.com/JoshuaSmyth/162ac4462325467fe8de2c4b7797187d
void copy_to_html_canvas(const unsigned char* ptr, int w, int h) 
{
  EM_ASM_(
    {
      var data = Module.HEAPU8.slice($0, $0 + $1 * $2 * 4);
      let context = Module['canvas'].getContext('2d');
      let imageData = context.getImageData(0, 0, $1, $2);
      imageData.data.set(data);
      context.putImageData(imageData, 0, 0);
    }, ptr, w, h);
}

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
    const auto project = [&](const cgfs::Position3D& v){ return p(M(v)); };

    for (const auto& t : model.faces)
      render_triangle(canvas, t, model.vertices | std::views::transform(project));
  }

  void render_scene(const MeshScene& scene, cgfs::Canvas& canvas, const cgfs::Camera& camera, const Viewport& vp)
  {
    const auto M_camera = cgfs::make_camera_matrix(camera.pose());

    for (const auto& I : scene.instances)
      render_model(I.model, canvas, vp, M_camera * I.transform);
  }
}


// https://wiki.libsdl.org/SDL3/README/emscripten#porting-your-app-to-emscripten
static void mainloop(void)   /* this will run often, possibly at the monitor's refresh rate */
{
  static int i = 0;
  const int N = 720;
  const auto theta = 2 * sp3::pi * i / N;

  const auto r = sp3::rotation{sp3::yhat, sp3::angle{theta}};

  const auto scene = MeshScene{std::vector<cgfs::Instance<cgfs::Mesh>>{
    {cgfs::wireframe_icosahedron(), sp3::transform{{-0.2f, 0.8f, 6.f}, r, 3.f}},
  }};
  cgfs::Canvas canvas{cgfs::Canvas::with_alpha_channel, {400, 400}, cgfs::Black};
  const auto camera = cgfs::Camera{ {{0, -1, 0}, {sp3::xhat, sp3::angle{-sp3::pi/12}}}} ;
  const auto viewport = Viewport{};

  render_scene(scene, canvas, camera, viewport);

  const auto& [w, h] = canvas.extent();
  copy_to_html_canvas(canvas.data(), w, h);

  i = (i + 1) % N;
}

int main()
{
  std::cout << "Executable build time: " << __TIMESTAMP__ << std::endl;

  emscripten_set_main_loop(mainloop, 0, 1);

  return 0;
}
