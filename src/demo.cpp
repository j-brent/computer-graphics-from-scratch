#include "bmp.h"
#include "canvas.h"
#include "color.h"
#include "extent.h"
#include "index.h"
#include "position.h"
#include "scene.h"
#include "sphere.h"

#include <limits>

namespace
{
  cgfs::Position3D canvas_to_viewport(cgfs::Index2D C_xy, cgfs::Extent2D C_wh, cgfs::Extent2D V_wh)
  {
    const float d = 1;
    auto& [Cx, Cy] = C_xy;
    auto& [Cw, Ch] = C_wh;
    auto& [Vw, Vh] = V_wh; 
    return {Cx * float(Vw)/Cw, Cy * float(Vh)/Ch, d};
  }

  void render(const cgfs::Scene& scene, cgfs::Canvas& canvas, const cgfs::Extent2D& viewport)
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
        const auto color = scene.trace_ray(O, V_xyz, 1, std::numeric_limits<float>::infinity());

        // 4. Paint the pixel that color
        canvas.putPixel({x, y}, color);
      }
  }
}

int main()
{
  const auto scene = cgfs::Scene{
    {
      cgfs::Sphere{{0, -1, 3}, 1, cgfs::Palette1::Orange},
      cgfs::Sphere{{2, 0, 4}, 1, cgfs::Palette1::Pink},
      cgfs::Sphere{{-2, 0, 4}, 1, cgfs::Palette1::Purple}
    }
  };

  const auto viewport = cgfs::Extent2D{1, 1};

  auto canvas = cgfs::Canvas{cgfs::Extent2D{400, 400}};

  render(scene, canvas, viewport);

  save_as_bmp(canvas, "output.bmp");

  return 0;
}