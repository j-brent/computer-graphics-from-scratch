add_library(cgfs STATIC
    bmp.h
    canvas.h
    canvas.cpp
    color.h
    extent.h
    index.h
    light.h
    position.h
    scene.h
    scene.cpp
    sphere.h
    triangle.h
)
target_include_directories(cgfs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cgfs::graphics ALIAS cgfs)

add_executable(demo 
    demo.cpp
)
target_link_libraries(demo cgfs::graphics bmpmini)

add_executable(demo-rasterizer
    demo-rasterizer.cpp
)
target_link_libraries(demo-rasterizer cgfs::graphics bmpmini)