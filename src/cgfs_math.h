#pragma once

#include <cmath>

namespace cgfs
{
  // convert float to int using std::round()
  inline int ftoi(float f)
  {
    return static_cast<int>(std::round(f));
  }
}
