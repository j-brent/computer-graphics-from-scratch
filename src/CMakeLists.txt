add_library(graphics STATIC
    bmp.h
    bmp.cpp
    camera.h
    canvas.h
    canvas.cpp
    cgfs_math.h
    color.h
    draw.h
    extent.h
    index.h
    instance.h
    interpolation.h
    light.h
    mesh.h
    position.h
    projection.h
    ray.h
    render.h
    scene.h
    scene.cpp
    sphere.h
    triangle.h
    viewport.h
)
add_library(cgfs::graphics ALIAS graphics)

target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(three-space REQUIRED)

target_link_libraries(graphics
  PUBLIC
    three-space::three-space
  PRIVATE
    bmpmini::bmpmini
)
