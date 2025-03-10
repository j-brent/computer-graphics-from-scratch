#include "bmp.h"
#include "canvas.h"
#include "extent.h"
#include "mesh.h"
#include "render.h"

int main()
{
  cgfs::Canvas canvas{{640, 640}};

  const auto viewport = cgfs::Extent2D{1, 1};
  const float d = 1;

  cgfs::render_instance(canvas, cgfs::Cube{{-1.5f, 0.f, 7.f}}, viewport, d);
  cgfs::render_instance(canvas, cgfs::Cube{{1.2f, 1.0f, 6.f}}, viewport, d);

  save_as_bmp(canvas, "figure10-3.bmp");
}