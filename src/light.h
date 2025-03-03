#pragma once

#include "position.h"

#include <memory>
#include <numeric>
#include <vector>

namespace cgfs
{
  class Light
  {
  public:
      template <typename T>
      Light(T t)
      : m_light{std::make_unique<owner_t<T>>(std::move(t))}
      {}

      Light(const Light& other)
      : m_light{other.m_light->clone()}
      {}

      Light& operator=(const Light& other)
      {
          Light copy{other};
          m_light.swap(copy.m_light);
          return *this;
      }

      Light(Light&&) = default;
      Light& operator=(Light&&) = default;

      ~Light() = default;

      float intensity(const Position3D& P, const Vector3D& N) const
      {
          return m_light->intensity2(P, N);
      }
  private:
      struct light_concept_t
      {
          virtual ~light_concept_t() = default;
          virtual std::unique_ptr<light_concept_t> clone() const = 0;
          virtual float intensity2(const Position3D& P, const Vector3D& N) const = 0;
      };

      template <typename Owned>
      struct owner_t : public light_concept_t
      {
          Owned o;

          owner_t(Owned o) : o{std::move(o)}{}

          std::unique_ptr<light_concept_t> clone() const override
          {
              return std::make_unique<owner_t>(*this);
          }

          float intensity2(const Position3D& P, const Vector3D& N) const override
          {
              return o.intensity(P, N);
          }
      };

      std::unique_ptr<light_concept_t> m_light;
  };
  
    class AmbientLight
    {
      public:
        AmbientLight(float intensity) : intensity_{intensity} {}

        float intensity(const Position3D&, const Vector3D&) const
        {
            return intensity_;
        }

      private:
        float intensity_;
    };

    class PointLight
    {
      public:
        PointLight(float intensity, Position3D pos) : position{pos}, intensity_{intensity} {}

        float intensity(const Position3D& P = {}, const Vector3D& N = {}) const
        {
            Vector3D L = position - P;
            float dotLN = dot(L, N);
            if (dotLN > 0)
            {
                return intensity_ * dotLN / (length(L) * length(N));
            }
            return 0;
        }

      private:
        Position3D position;
        float intensity_;
    };

    class DirectionalLight
    {
      public:
        DirectionalLight(float intensity, Vector3D dir) : direction{dir}, intensity_{intensity} {}

        float intensity(const Position3D& P = {}, const Vector3D& N = {}) const
        {
            float dotLN = dot(direction, N);
            if (dotLN > 0)
            {
                return intensity_ * dotLN / (length(direction) * length(N));
            }
            return 0;
        }
      private:
        Vector3D direction;
        float intensity_;
    };

    inline float compute_lighting(const Position3D& P, const Vector3D& N, const std::vector<Light>& lights)
    {
        return std::accumulate(lights.begin(), lights.end(), 0.0f, [&](float acc, const Light& light) {
            return acc + light.intensity(P, N);
        });
    }
    
} // namespace cgfs