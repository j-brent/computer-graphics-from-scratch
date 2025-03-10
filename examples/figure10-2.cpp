#include "bmp.h"
#include "canvas.h"
#include "color.h"
#include "extent.h"
#include "position.h"
#include "mesh.h"
#include "render.h"

#include <vector>

int main()
{
  cgfs::Canvas canvas{{640, 640}};

  const auto viewport = cgfs::Extent2D{1, 1};
  const float d = 1;

  const auto T = cgfs::Vector3D{-1.5, 0, 7};
  // Vertices
  auto vertices = std::vector<cgfs::Position3D> {
    cgfs::Position3D{ 1, 1, 1} + T,
    cgfs::Position3D{-1, 1, 1} + T,
    cgfs::Position3D{-1, -1, 1} + T,
    cgfs::Position3D{ 1, -1, 1} + T,
    cgfs::Position3D{ 1, 1, -1} + T,
    cgfs::Position3D{-1, 1, -1} + T,
    cgfs::Position3D{-1, -1, -1} + T,
    cgfs::Position3D{ 1, -1, -1} + T,
  };
  // Triangles
  auto faces = std::vector<cgfs::Mesh::TFace>{
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
  };

  const auto cube = cgfs::Mesh{std::move(vertices), std::move(faces)};
  cgfs::render_object(canvas, cube, viewport, d);

  save_as_bmp(canvas, "figure10-2.bmp");
}