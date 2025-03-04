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
    
    // Returns the sphere closest to the origin of the ray and the value of t at the point of intersection.
    // If no sphere is intersected, the first element of the pair is cgfs::null_sphere.
    std::pair<Sphere, float> closest_intersection(const Ray3D& ray, const std::vector<Sphere>& spheres)
    {
      auto closest_t = std::numeric_limits<float>::infinity();
      auto closest_sphere = cgfs::Sphere{};
      const auto& t_min = ray.t_min;
      const auto& t_max = ray.t_max;
      for(const auto& sphere : spheres)
      {
        const auto [t1, t2] = intersect_ray_sphere(ray.origin, ray.direction, sphere);
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


  // sp point on the surface
  // V is the "view vector" (the direction from the surface point to the camera)
  inline float compute_lighting(const SurfacePoint& sp, const Vector3D& V, const std::vector<Light>& lights, const std::vector<Sphere>& obstacles)
  {
    auto blocked = [&](const Light& light) -> bool
    {
      auto ray = light.back_ray(sp.pos);
      if (ray == cgfs::null_ray3d)
        return false;
      ray.t_min = 0.001f;
      const auto [shadow_sphere, shadow_t] = closest_intersection(ray, obstacles);
      return shadow_sphere != cgfs::null_sphere;
    };

    return std::accumulate(lights.begin(), lights.end(), 0.0f, [&](float acc, const Light& light) {
        return blocked(light) ? acc : acc + light.intensity(sp, V);
    });
  }

  // Reflect the vector R around the normal N
  inline Vector3D reflect(const Vector3D& R, const Vector3D& N)
  {
    return 2 * dot(R, N) * N - R;
  }
}

namespace cgfs
{
  Color Scene::trace_ray(const Ray3D& ray, size_t recursion_depth) const
  {
    const auto [closest_sphere, closest_t] = closest_intersection(ray, m_spheres);

    if (closest_sphere == cgfs::null_sphere)
    {
      return BACKGROUND_COLOR;
    }

    const auto& O = ray.origin;
    const auto& D = ray.direction;
    const auto P = O + closest_t * D; // Compute intersection
    auto N = P - closest_sphere.center; // Compute sphere normal at intersection
    N = N / length(N);

    const auto sp = SurfacePoint{P, N, closest_sphere.specular};
    const auto local_color = closest_sphere.color * compute_lighting(sp, -D, m_lights, m_spheres);

    const auto r = closest_sphere.reflective;
    if (recursion_depth == 0 || r == 0)
    {
      return local_color;
    }

    const auto reflected_ray = Ray3D{sp.pos, reflect(-ray.direction, sp.normal), 0.001f, std::numeric_limits<float>::infinity()};
    const auto reflected_color = trace_ray(reflected_ray, recursion_depth - 1);

    return (1 - r) * local_color + r * reflected_color;
  }
}