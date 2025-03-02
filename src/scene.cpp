#include "scene.h"

#include <cmath>
#include <limits>
#include <utility>

namespace cgfs
{
  namespace
  {
    // constexpr cgfs::Color BACKGROUND_COLOR = {56, 62, 66};
    constexpr cgfs::Color BACKGROUND_COLOR = Palette1::DarkGray;

    std::pair<float, float> intersect_ray_sphere(Position3D O, Vector3D D, const Sphere& sphere)
    {
      const auto OC = O - sphere.center;
      const auto k1 = dot(D, D);
      const auto k2 = 2 * dot(OC, D);
      const auto k3 = dot(OC, OC) - sphere.radius * sphere.radius;
      const auto discriminant = k2 * k2 - 4 * k1 * k3;
      if(discriminant < 0) return {std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};
      const auto t1 = (-k2 + std::sqrt(discriminant)) / (2 * k1);
      const auto t2 = (-k2 - std::sqrt(discriminant)) / (2 * k1);
      return {t1, t2};
    }
  }
}

namespace cgfs
{
  Color Scene::trace_ray(Position3D O, Position3D D, float t_min, float t_max) const
  {
    auto closest_t = std::numeric_limits<float>::infinity();
    auto closest_sphere = cgfs::Sphere{};
    for(const auto& sphere : m_spheres)
    {
      const auto [t1, t2] = intersect_ray_sphere(O, D-O, sphere);
      if(t_min < t1 && t1 < t_max && t1 < closest_t)
      {
        closest_t = t1;
        closest_sphere = sphere;
      }
      if(t_min < t2 && t2 < t_max && t2 < closest_t)
      {
        closest_t = t2;
        closest_sphere = sphere;
      }
    }
    if (closest_sphere == cgfs::Sphere{})
    {
      return BACKGROUND_COLOR;
    }
    return closest_sphere.color;
  }
}