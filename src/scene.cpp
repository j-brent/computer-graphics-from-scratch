#include "scene.h"

#include <cmath>
#include <limits>
#include <utility>

namespace cgfs
{
  namespace
  {
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

    std::pair<Sphere, float> closest_intersection(Position3D O, Vector3D D, float t_min, float t_max, const std::vector<Sphere>& spheres)
    {
      auto closest_t = std::numeric_limits<float>::infinity();
      auto closest_sphere = cgfs::Sphere{};
      for(const auto& sphere : spheres)
      {
        const auto [t1, t2] = intersect_ray_sphere(O, D, sphere);
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

      return {closest_sphere, closest_t};
    }
  }

  std::pair<Sphere, float> closest_intersection(const Ray3D& ray, const std::vector<Sphere>& spheres)
  {
    return closest_intersection(ray.origin, ray.direction, ray.t_min, ray.t_max, spheres);
  }

  // P point on the surface
  // N normal at P
  // V is the "view vector" (the direction from the point P to the camera)
  // s is the specular component
  inline float compute_lighting(const Position3D& P, const Vector3D& N, const Vector3D& V, int s, const std::vector<Light>& lights, const std::vector<Sphere>& obstacles)
  {
      auto blocked = [&](const Light& light) -> bool
      {
        auto ray = light.back_ray(P);
        if (ray == cgfs::null_ray3d)
          return false;
        ray.t_min = 0.001f;
        const auto [shadow_sphere, shadow_t] = closest_intersection(ray, obstacles);
        // const auto [shadow_sphere, shadow_t] = closest_intersection(P, L, 0.001f, t_max, obstacles);
        return shadow_sphere != Sphere{};
      };

      return std::accumulate(lights.begin(), lights.end(), 0.0f, [&](float acc, const Light& light) {
          return blocked(light) ? acc : acc + light.intensity(P, N, V, s);
      });
  }
}

namespace cgfs
{
  // Color Scene::trace_ray(Position3D O, Vector3D D, float t_min, float t_max) const
  Color Scene::trace_ray(const Ray3D& ray) const
  {
    const auto [closest_sphere, closest_t] = closest_intersection(ray, m_spheres);
    // const auto [closest_sphere, closest_t] = closest_intersection(O, D, t_min, t_max, m_spheres);

    if (closest_sphere == cgfs::Sphere{})
    {
      return BACKGROUND_COLOR;
    }

    const auto& O = ray.origin;
    const auto& D = ray.direction;

    const auto P = O + closest_t * D; // Compute intersection
    auto N = P - closest_sphere.center; // Compute sphere normal at intersection
    N = N / length(N);
    return closest_sphere.color * compute_lighting(P, N, -D, closest_sphere.specular, m_lights, m_spheres);
  }
}