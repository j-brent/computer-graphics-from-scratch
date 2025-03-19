cmake_minimum_required(VERSION 3.23)

project(cgfs
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/CPM.cmake)
CPMAddPackage("gh:j-brent/three-space#d04f940e2da32f06e8de0dd3ef80623fc313a600")

add_subdirectory(ext)
add_subdirectory(src)

include(cmake/building_with_emscripten.cmake)

if (PROJECT_IS_TOP_LEVEL)
  if(NOT CGFS_BUILDING_WITH_EMSCRIPTEN)
    add_subdirectory(test)
    add_subdirectory(examples)
  else()
    add_subdirectory(examples/browser)
  endif()
endif()

