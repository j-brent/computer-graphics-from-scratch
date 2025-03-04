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

      /*
      *   Trace a ray through the scene and return the color of the first object hit by the ray.
      *   If no object is hit, return the background color.
      */
      Color trace_ray(const Ray3D& ray) const;

    private:
      std::vector<Sphere> m_spheres;
      std::vector<Light> m_lights;
  };
}