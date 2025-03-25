
#include "bmp.h"
#include "canvas.h"
#include "index.h"
#include "position.h"
#include "render.h"

#include <vector>

int main()
{
  cgfs::Canvas canvas{{640, 640}};

  const auto viewport = cgfs::Extent2D{1, 1};
  const float d = 1;

  // The four "front" vertices
  const auto vAf = cgfs::Position3D{-1-2, 1, 1};
  const auto vBf = cgfs::Position3D{ 1-2, 1, 1};
  const auto vCf = cgfs::Position3D{ 1-2, -1, 1};
  const auto vDf = cgfs::Position3D{-1-2, -1, 1};
  // The four "back" vertices
  const auto vAb = cgfs::Position3D{-1-2, 1, 2};
  const auto vBb = cgfs::Position3D{ 1-2, 1, 2};
  const auto vCb = cgfs::Position3D{ 1-2, -1, 2};
  const auto vDb = cgfs::Position3D{-1-2, -1, 2};
    
  const auto project = [&](const cgfs::Position3D& v){
    return cgfs::detail::project_vertex(v, d, viewport, canvas.extent()); 
  };
  
  const auto front_face = std::vector<std::pair<cgfs::Index2D, cgfs::Index2D>>{
    {project(vAf), project(vBf)},
    {project(vBf), project(vCf)},
    {project(vCf), project(vDf)},
    {project(vDf), project(vAf)},
  };
  for (const auto& [a, b] : front_face)
    cgfs::draw_line(canvas, a, b, cgfs::Blue);

  const auto back_face = std::vector<std::pair<cgfs::Index2D, cgfs::Index2D>>{
    {project(vAb), project(vBb)},
    {project(vBb), project(vCb)},
    {project(vCb), project(vDb)},
    {project(vDb), project(vAb)},
  };
  for (const auto& [a, b] : back_face)
    cgfs::draw_line(canvas, a, b, cgfs::Red);

  const auto front_to_back_edges = std::vector<std::pair<cgfs::Index2D, cgfs::Index2D>>{
    {project(vAf), project(vAb)},
    {project(vBf), project(vBb)},
    {project(vCf), project(vCb)},
    {project(vDf), project(vDb)},
  };
  for (const auto& [a, b] : front_to_back_edges)
    cgfs::draw_line(canvas, a, b, cgfs::Green);

  save_as_bmp(canvas, "figure9-4.bmp");
    
  return 0;
}