#include "bmp.h"
#include "canvas.h"
#include "color.h"
#include "render.h"

int main()
{
  cgfs::Canvas canvas{{640, 640}};
  
  cgfs::draw_line(canvas, {-200, -100}, {240, 120}, cgfs::Palette1::Orange);
  cgfs::draw_line(canvas, {-50, -200}, {60, 240}, cgfs::Palette1::Pink);

  save_as_bmp(canvas, "figure6-5.bmp");
  
  return 0;
}