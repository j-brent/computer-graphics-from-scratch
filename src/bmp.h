#pragma once

#include <string_view>

namespace cgfs
{
  class Canvas;
  
  void save_as_bmp(const Canvas& canvas, std::string_view filename);
}