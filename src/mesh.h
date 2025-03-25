#pragma once

#include "color.h"
#include "position.h"
#include "triangle.h"

#include "sp3/transform.h"

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

    // faces as triangles (Triangle3D) in model space
    auto triangles() const
    {
      const auto to_triangle = [this](const TFace& t){
        return Triangle3D{vertices[t.a], vertices[t.b], vertices[t.c], t.col};
      };
      return faces | std::ranges::views::transform(to_triangle);
    }

    // faces as triangles (Triangle3D) with xform applied to the vertices (whose coordinates are in model space)
    auto triangles(sp3::transform xform) const
    {
      const auto to_triangle = [xform = std::move(xform), this](const TFace& t){
        return Triangle3D{
          xform(vertices[t.a]),
          xform(vertices[t.b]),
          xform(vertices[t.c]),
          t.col
        };
      };
      return faces | std::ranges::views::transform(to_triangle);
    }
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

  inline Mesh wireframe_tetrahedron()
  {
    /*
      1  1  1   -1  1 -1    1 -1 -1  (0, 1, 2)
     -1  1 -1   -1 -1  1    1 -1 -1  (1, 3, 2)
      1  1  1    1 -1 -1   -1 -1  1  (0, 2, 3)
      1  1  1   -1 -1  1   -1  1 -1  (0, 3, 1)
    */
    const float a = 1.f / 2;
    return {std::vector<Position3D> {
      { a, a, a},
      {-a, a, -a},
      {a, -a, -a},
      {-a, -a, a},
    }, std::vector<Mesh::TFace>{
      {0, 1, 2, cgfs::Red},
      {1, 3, 2, cgfs::Green},
      {0, 2, 3, cgfs::Blue},
      {0, 3, 1, cgfs::Purple},
    }};
  } 

  inline Mesh wireframe_octahedron()
  {
    /*
      -a  0  a   -a  0 -a    0  b  0   (0, 1, 2)
      -a  0 -a    a  0 -a    0  b  0   (1, 3, 2)
       a  0 -a    a  0  a    0  b  0   (3, 4, 2)
       a  0  a   -a  0  a    0  b  0   (4, 0, 2)
       a  0 -a   -a  0 -a    0 -b  0   (3, 1, 5)
      -a  0 -a   -a  0  a    0 -b  0   (1, 0, 5)
       a  0  a    a  0 -a    0 -b  0   (4, 3, 5)
      -a  0  a    a  0  a    0 -b  0   (0, 4, 5)
      Where a = 1 / (2 * sqrt(2)) and b = 1 / 2
    */
   const float a = 1. / (2 * std::sqrt(2));
   const float b = 1. / 2;
   return {std::vector<Position3D> {
      {-a, 0, a},
      {-a, 0, -a},
      {0, b, 0},
      {a, 0, -a},
      {a, 0, a},
      {0, -b, 0},
    }, std::vector<Mesh::TFace>{
      {0, 1, 2, cgfs::Red},
      {1, 3, 2, cgfs::Green},
      {3, 4, 2, cgfs::Blue},
      {4, 0, 2, cgfs::Purple},
      {3, 1, 5, cgfs::Yellow},
      {1, 0, 5, cgfs::Green},
      {4, 3, 5, cgfs::Orange},
      {0, 4, 5, cgfs::Cyan},
    }};
  }


  inline Mesh wireframe_icosahedron()
  {
    /*
       0  b -a    b  a  0   -b  a  0  (0, 1, 2)
       0  b  a   -b  a  0    b  a  0  (3, 2, 1)
       0  b  a    0 -b  a   -a  0  b  (3, 4, 5)
       0  b  a    a  0  b    0 -b  a  (3, 6, 4)
       0  b -a    0 -b -a    a  0 -b  (0, 7, 8)
       0  b -a   -a  0 -b    0 -b -a  (0, 9, 7)
       0 -b  a    b -a  0   -b -a  0  (4, 10, 11)
       0 -b -a   -b -a  0    b -a  0  (7, 11, 10)
      -b  a  0   -a  0  b   -a  0 -b  (2, 5, 9)
      -b -a  0   -a  0 -b   -a  0  b  (11, 9, 5)
       b  a  0    a  0 -b    a  0  b  (1, 8, 6)
       b -a  0    a  0  b    a  0 -b  (10, 6, 8)
       0  b  a   -a  0  b   -b  a  0  (3, 5, 2)
       0  b  a    b  a  0    a  0  b  (3, 1, 6)
       0  b -a   -b  a  0   -a  0 -b  (0, 2, 9)
       0  b -a    a  0 -b    b  a  0  (0, 8, 1)
       0 -b -a   -a  0 -b   -b -a  0  (7, 9, 11)
       0 -b -a    b -a  0    a  0 -b  (7, 10, 8)
       0 -b  a   -b -a  0   -a  0  b  (4, 11, 5)
       0 -b  a    a  0  b    b -a  0  (4, 6, 10)
       Where a = 1 / 2 and b = 1 / (2 * phi)
       phi is the golden ratio = (1 + sqrt(5)) / 2
    */
    const float a = 1.f / 2;
    const float two_phi = 1.f + std::sqrt(5);
    const float b = 1.f / two_phi;
    return {std::vector<Position3D> {
        { 0,  b, -a},
        { b,  a,  0},
        {-b,  a,  0},
        { 0,  b,  a},
        { 0, -b,  a},
        {-a,  0,  b},
        { a,  0,  b},
        { 0, -b, -a},
        { a,  0, -b},
        {-a,  0, -b},
        { b, -a,  0},
        {-b, -a,  0},
      }, std::vector<Mesh::TFace>{
        {0, 1, 2, cgfs::Red},
        {3, 2, 1, cgfs::Orange},
        {3, 4, 5, cgfs::Yellow},
        {3, 6, 4, cgfs::Green},
        {0, 7, 8, cgfs::Green},
        {0, 9, 7, cgfs::Blue},
        {4, 10, 11, cgfs::Purple},
        {7, 11, 10, cgfs::Cyan},
        {2, 5, 9, cgfs::Red},
        {11, 9, 5, cgfs::Orange},
        {1, 8, 6, cgfs::Yellow},
        {10, 6, 8, cgfs::Green},
        {3, 5, 2, cgfs::Blue},
        {3, 1, 6, cgfs::Purple},
        {0, 2, 9, cgfs::Cyan},
        {0, 8, 1, cgfs::Red},
        {7, 9, 11, cgfs::Orange},
        {7, 10, 8, cgfs::Yellow},
        {4, 11, 5, cgfs::Green},
        {4, 6, 10, cgfs::Blue},
      }};
  }

#if 0
  inline Mesh wireframe_dodecahedron()
  {
    /*
       c  0  1   -c  0  1   -b  b  b    0  1  c    b  b  b
      -c  0  1    c  0  1    b -b  b    0 -1  c   -b -b  b
       c  0 -1   -c  0 -1   -b -b -b    0 -1 -c    b -b -b
      -c  0 -1    c  0 -1    b  b -b    0  1 -c   -b  b -b
       0  1 -c    0  1  c    b  b  b    1  c  0    b  b -b
       0  1  c    0  1 -c   -b  b -b   -1  c  0   -b  b  b
       0 -1 -c    0 -1  c   -b -b  b   -1 -c  0   -b -b -b
       0 -1  c    0 -1 -c    b -b -b    1 -c  0    b -b  b
       1  c  0    1 -c  0    b -b  b    c  0  1    b  b  b
       1 -c  0    1  c  0    b  b -b    c  0 -1    b -b -b
      -1  c  0   -1 -c  0   -b -b -b   -c  0 -1   -b  b -b
      -1 -c  0   -1  c  0   -b  b  b   -c  0  1   -b -b  b
      Where b = 1 / phi and c = 2 - phi
      Divide each coordinate by 2.
    */
    const float phi = (1.f + std::sqrt(5)) / 2;
    const float b = (1.f / phi) / 2;
    const float c = (2.f - phi) / 2;
  }
#endif

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
