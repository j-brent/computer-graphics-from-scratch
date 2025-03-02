add_library(cgfs STATIC
    bmp.h
    canvas.h
    canvas.cpp
    color.h
    extent.h
    index.h
    position.h
    scene.h
    scene.cpp
    sphere.h
)

add_executable(demo 
    demo.cpp
)
target_link_libraries(demo cgfs bmpmini)