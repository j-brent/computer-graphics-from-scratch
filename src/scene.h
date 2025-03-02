#pragma once

#include "color.h"
#include "position.h"
#include "sphere.h"

#include <vector>

namespace cgfs
{
  class Scene
  {
    public:
      Scene(std::vector<Sphere> spheres) : m_spheres{spheres} {}

      Color trace_ray(Position3D O, Position3D D, float t_min, float t_max) const;

    private:
      std::vector<Sphere> m_spheres;
  };
}