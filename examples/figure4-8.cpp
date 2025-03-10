#include "bmp.h"
#include "canvas.h"
#include "color.h"
#include "light.h"
#include "render.h"
#include "scene.h"
#include "sphere.h"

int main()
{
  const auto scene = cgfs::Scene{
    {
      cgfs::Sphere{{0, -1, 3}, 1, cgfs::Palette1::Orange, 500 /* shiny */, 0.2f},
      cgfs::Sphere{{2, 0, 4}, 1, cgfs::Palette1::Pink, 500 /* shiny */, 0.3f},
      cgfs::Sphere{{-2, 0, 4}, 1, cgfs::Palette1::Purple, 10 /* somewhat shiny */, 0.4f},
      cgfs::Sphere{{0, -5001, 0}, 5000, cgfs::Palette1::Yellow, 1000 /* very shiny */, 0.5f}
    },
    {
      cgfs::AmbientLight{0.2f},
      cgfs::PointLight{0.6f, {2, 1, 0}},
      cgfs::DirectionalLight{0.2f, {1, 4, 4}}
    }
  };

  const auto viewport = cgfs::Extent2D{1, 1};

  auto canvas = cgfs::Canvas{cgfs::Extent2D{400, 400}};

  render(scene, canvas, viewport);

  save_as_bmp(canvas, "figure4-8.bmp");

  return 0;
}