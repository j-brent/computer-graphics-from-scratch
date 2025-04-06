#include "bmp.h"
#include "camera.h"
#include "canvas.h"
#include "extent.h"
#include "instance.h"
#include "mesh.h"
#include "render.h"
#include "scene.h"

#include "sp3/angle.h"
#include "sp3/axes.h"
#include "sp3/transform.h"

int main()
{
  auto canvas = cgfs::Canvas{{640, 640}, cgfs::Palette1::DarkGray};

  const auto scene = cgfs::MeshScene{
    std::vector<cgfs::Instance<cgfs::Mesh>>{
      {cgfs::wireframe_cube(), sp3::transform{{-1.5f, 0.f, 7.f}, {sp3::yhat, sp3::angle{sp3::pi/12}}}},
      {cgfs::wireframe_cube(), sp3::transform{{1.2f, 1.0f, 6.f}, {sp3::yhat, sp3::angle{-sp3::pi/12}}}},
    },
    std::vector<cgfs::Light>{
      cgfs::AmbientLight{0.2f},
      cgfs::PointLight{0.6f, {-1, 1, 0}},
      cgfs::DirectionalLight{0.2f, {0, 0, 0}}
    }
  };

  const auto camera = cgfs::Camera{
    sp3::pose{{0, -2, -1}, {sp3::xhat, sp3::angle{-sp3::pi/12}}},
    cgfs::Viewport{{1, 1}, 1}
  };

  cgfs::render_scene(canvas, scene, camera);

  cgfs::save_as_bmp(canvas, "figure13-1.bmp");
}
