Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        914aeecfe23b1e16af6ea675a4fb5dbd5a5b8d0a # v3.8.0 (latest as of 2025-03-03)
)

FetchContent_MakeAvailable(Catch2)

add_executable(cgfs-tests
  canvas_tests.cpp
  interpolation_tests.cpp
  # light_tests.cpp
)
target_link_libraries(cgfs-tests 
  PUBLIC
    cgfs::graphics
  PRIVATE
    Catch2::Catch2WithMain
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(cgfs-tests)