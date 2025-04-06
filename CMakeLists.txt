cmake_minimum_required(VERSION 3.23)

project(cgfs
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/CPM.cmake)
CPMAddPackage("gh:j-brent/three-space#05df82050e46a4fc284ba91597cdcbf8bd00e3e7")

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

