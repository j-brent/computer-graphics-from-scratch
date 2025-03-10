add_library(cgfs STATIC
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
target_include_directories(cgfs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cgfs::graphics ALIAS cgfs)

target_link_libraries(cgfs PRIVATE bmpmini)