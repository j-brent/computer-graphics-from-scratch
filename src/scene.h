#pragma once

#include "color.h"
#include "light.h"
#include "position.h"
#include "sphere.h"

#include <vector>

namespace cgfs
{
  class Scene
  {
    public:
      explicit Scene(std::vector<Sphere> spheres, std::vector<Light> lights = {})
      : m_spheres{std::move(spheres)}
      {
        m_lights = std::move(lights);
      }

      Color trace_ray(Position3D O, Position3D D, float t_min, float t_max) const;

    private:
      std::vector<Sphere> m_spheres;
      std::vector<Light> m_lights;
  };
}