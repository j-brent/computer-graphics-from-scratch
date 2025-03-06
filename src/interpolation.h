#pragma once

#include "index.h"

#include <cmath>
#include <vector>

namespace cgfs
{

  // Interpolate between the dependent variables d0 and d1 along the 'axis' of the independent variables i0 and i1
  std::vector<int> interpolate(int i0, int d0, int i1, int d1)
  {
    auto values = std::vector<int>{};
    values.reserve(i1 - i0 + 1);

    const auto a = static_cast<float>(d1 - d0) / (i1 - i0);
    auto d = static_cast<float>(d0);
    for (int i = i0; i <= i1; ++i, d += a)
      values.push_back(static_cast<int>(std::round(d)));

    return values;
  }

  std::vector<float> interpolatef(int i0, float d0, int i1, float d1)
  {
    auto values = std::vector<float>{};
    values.reserve(i1 - i0 + 1);

    const auto a = (d1 - d0) / (i1 - i0);
    auto d = d0;
    for (int i = i0; i <= i1; ++i, d += a)
      values.push_back(d);

    return values;
  }

  namespace detail
  {
      // Interpolate between two points a and b along the x-axis 
      inline std::vector<Index2D> interpolate_x(const Index2D& a, const Index2D& b, float slope)
      {
        if(b.x < a.x) // make sure a.x < b.x
        return interpolate_x(b, a, slope);
        
        std::vector<Index2D> result;
        result.reserve(b.x - a.x + 1);
        
        auto y = static_cast<float>(a.y);
        const auto dy = slope;
        for (auto x = a.x; x <= b.x; ++x, y+=dy)
          result.push_back({x, static_cast<int>(std::round(y))});
        
        return result;
      }
      
      // Interpolate between two points a and b along the y-axis 
      inline std::vector<Index2D> interpolate_y(const Index2D& a, const Index2D& b, float slope)
      {
        if(b.y < a.y) // make sure a.y < b.y
          return interpolate_y(b, a, slope);
  
        std::vector<Index2D> result;
        result.reserve(b.y - a.y + 1);
        
        auto x = static_cast<float>(a.x);
        const auto dx = 1.f / slope;
        
        for (auto y = a.y; y <= b.y; ++y, x+=dx)
          result.push_back({static_cast<int>(std::round(x)), y});
  
        return result;
      }
    }
  
      inline std::vector<Index2D> interpolate(const Index2D& a, const Index2D& b)
      {
        const auto [dx, dy] = b - a;
        // TODO: handle case where dx = 0
        const auto slope = static_cast<float>(dy) / dx;
  
        if (std::abs(slope) < 1) // line is horizontal-ish
            return detail::interpolate_x(a, b, slope);
        else // line is vertical-ish
            return detail::interpolate_y(a, b, slope);
      }  
}