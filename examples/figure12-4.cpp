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
  auto canvas = cgfs::Canvas{{640, 640}};

  const auto scene = cgfs::MeshScene{std::vector<cgfs::Instance>{
    {cgfs::wireframe_cube(), sp3::transform{{-1.5f, 0.f, 7.f}, {}}},
    {cgfs::wireframe_cube(), sp3::transform{{1.2f, 1.0f, 6.f}, {}}},
  }};

  const auto camera = cgfs::Camera{
    sp3::pose{{0, -1, 0}, {sp3::xhat, sp3::angle{-sp3::pi/12}}},
    cgfs::Viewport{{1, 1}, 1}
  };

  render_scene(canvas, scene, camera);

  save_as_bmp(canvas, "figure12-4.bmp");
}
