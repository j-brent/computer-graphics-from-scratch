#pragma once

#include "extent.h"
#include "index.h"
#include "cgfs_math.h"
#include "position.h"
#include "viewport.h"

#include <cmath>

namespace cgfs
{
  // The projection operator from camera to canvas coordinates
  class Projection
  {
  public:
    explicit Projection(Extent2D canvas_dimensions, Viewport vp)
    : m_canvas_dimensions{std::move(canvas_dimensions)}
    , m_viewport{std::move(vp)}
    {}
    
    // project the 3D pt p (in camera space)
    // onto the 2D canvas (integer coords)
    // via the 2D viewport (real coords)
    Index2D operator()(const Position3D& p_camera) const
    {
      const auto& [vx, vy, vz] = p_camera;
      const auto& [Cw, Ch] = m_canvas_dimensions;
      const auto& [Vw, Vh] = m_viewport.size;
      const auto d_prime = m_viewport.distance / vz;
      return {
        cgfs::ftoi((Cw / Vw) * d_prime * vx),
        cgfs::ftoi((Ch / Vh) * d_prime * vy)
      };
    }

  private:
    Extent2D m_canvas_dimensions = {0, 0};
    Viewport m_viewport = {{1, 1}, 1};
  };
}
