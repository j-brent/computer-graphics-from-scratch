#pragma once

#include "projection.h"
#include "viewport.h"

#include "sp3/pose.h"
#include "sp3/transform.h"

namespace cgfs
{
  class Camera
  {
  public:
    Camera() = default;
    ~Camera() = default;

    explicit Camera(sp3::pose pose, Viewport vp = {})
    : m_pose{std::move(pose)}
    , m_viewport{std::move(vp)}
    {}

    const sp3::pose& pose() const { return m_pose; }

    Projection projection(const cgfs::Extent2D& canvas_extent) const
    {
      return Projection{canvas_extent, m_viewport};
    }

  private:
    sp3::pose m_pose = {};
    Viewport m_viewport = {};
  };

  inline sp3::transform make_camera_matrix(const sp3::pose& camera_pose)
  {
    const auto O = sp3::point{0, 0, 0};
    return {O - camera_pose.position, camera_pose.orientation.inverse()};
  }
}
