#include "bmp.h"
#include "canvas.h"
#include "color.h"
#include "render.h"
#include "triangle.h"

int main()
{
  cgfs::Canvas canvas{{640, 640}};

  // Figure 8.1
  cgfs::Vertex2D v0{{-200, -250}, cgfs::Palette1::Pink, 0.5f};
  cgfs::Vertex2D v1{{200, 50}, cgfs::Palette1::Pink, 0.0f};
  cgfs::Vertex2D v2{{20, 250}, cgfs::Palette1::Pink, 1.0f};
  cgfs::draw_shaded_triangle(canvas, {v0, v1, v2});


  save_as_bmp(canvas, "figure8-1.bmp");
  
  return 0;
}