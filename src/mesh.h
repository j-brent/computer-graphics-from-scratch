#pragma once

#include "color.h"
#include "position.h"

#include <ranges>
#include <vector>

namespace cgfs
{
  struct Mesh
  {
    struct TFace
    {
      size_t a;
      size_t b;
      size_t c;
      Color col;
    };
    std::vector<Position3D> vertices;
    std::vector<TFace> faces;
  };

  inline Mesh wireframe_cube()
  {
    return {std::vector<Position3D> {
      { 1, 1, 1},
      {-1, 1, 1},
      {-1, -1, 1},
      { 1, -1, 1},
      { 1, 1, -1},
      {-1, 1, -1},
      {-1, -1, -1},
      { 1, -1, -1},
    }, std::vector<Mesh::TFace>{
      {0, 1, 2, cgfs::Red},
      {0, 2, 3, cgfs::Red},
      {4, 0, 3, cgfs::Green},
      {4, 3, 7, cgfs::Green},
      {5, 4, 7, cgfs::Blue},
      {5, 7, 6, cgfs::Blue},
      {1, 5, 6, cgfs::Yellow},
      {1, 6, 2, cgfs::Yellow},
      {4, 5, 1, cgfs::Purple},
      {4, 1, 0, cgfs::Purple},
      {2, 6, 7, cgfs::Cyan},
      {2, 7, 3, cgfs::Cyan},
    }};
  }

  class Cube
  {
  public:

    Cube() = default;
    explicit Cube(Vector3D pos) : m_pos{std::move(pos)}{}
    ~Cube() = default;

    Cube(const Cube&) = default;
    Cube& operator=(const Cube&) = default;

    Cube(Cube&&) = default;
    Cube& operator=(Cube&&) = default;

    Position3D operator[](size_t i) const
    {
      return m_mesh.vertices.at(i) + m_pos;
    }

    auto vertices() const{
      return m_mesh.vertices | std::ranges::views::transform([t = m_pos](const Position3D& p){ return p + t; });
    }

    const std::vector<Mesh::TFace>& faces() const
    {
      return m_mesh.faces;
    }

  private:
    Vector3D m_pos = {0, 0, 0};
    Mesh m_mesh = wireframe_cube();
  };
}
