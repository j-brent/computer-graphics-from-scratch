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
      *   Trace a ray (O + tD) from the origin O in the direction D, and return the color of the first object hit by the ray.
      *   If no object is hit, return the background color.
      * 
      *   O: the origin of the ray (the position of the camera)
      *   D: the direction of the ray (the pixel position on the viewport, relative to the camera)
      *   t_min: the minimum distance to consider for intersections
      *   t_max: the maximum distance to consider for intersections
      * 
      *   Returns: the color of the first object hit by the ray, or the background color if no object is hit
      * 
      *   Note: t<0         behind the camera
      *         t \in [0,1] between the camera and the projection plane/viewport
      *         t>1         in front of the projection plane/viewport
      */
      // Color trace_ray(Position3D O, Vector3D D, float t_min, float t_max) const;
      Color trace_ray(const Ray3D& ray) const;

    private:
      std::vector<Sphere> m_spheres;
      std::vector<Light> m_lights;
  };
}