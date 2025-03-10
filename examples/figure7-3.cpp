#include "bmp.h"
#include "canvas.h"
#include "color.h"
#include "render.h"


int main()
{
  cgfs::Canvas canvas{{640, 640}};

  // Figure 7.1, 7.3
  cgfs::draw_filled_triangle(canvas, {-200, -250}, {200, 50}, {20, 250}, cgfs::Palette1::Yellow);
  cgfs::draw_wireframe_triangle(canvas, {-200, -250}, {200, 50}, {20, 250}, cgfs::Palette1::Purple);

  save_as_bmp(canvas, "figure7-3.bmp");
  
  return 0;
}