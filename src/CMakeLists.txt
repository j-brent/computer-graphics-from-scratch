add_library(graphics STATIC
    bmp.h
    bmp.cpp
    canvas.h
    canvas.cpp
    color.h
    extent.h
    index.h
    light.h
    position.h
    render.h
    scene.h
    scene.cpp
    sphere.h
    triangle.h
)
add_library(cgfs::graphics ALIAS graphics)

target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(graphics PRIVATE bmpmini)