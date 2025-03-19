#include <catch2/catch_test_macros.hpp>

#include "canvas.h"

#include <algorithm>
#include <span>

TEST_CASE("Constructing a canvas")
{
  SECTION("RGB - Single color uniform canvas")
  {
    const auto c = cgfs::Canvas{{11, 7}, cgfs::Color{1, 2, 3}};
    
    const std::vector<unsigned char> expected = {
      1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,
      1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,
      1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,
      1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,
      1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,
      1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,
      1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3,  1, 2, 3
    };
    
    const auto result = std::span{c.data(), c.num_bytes()};
    
    CHECK(std::equal(result.begin(), result.end(), expected.begin()));
  }
  
  SECTION("RGB - put pixel")
  {
    auto c = cgfs::Canvas{{7, 5}, cgfs::Color{0, 0, 0}};

    const auto center = cgfs::Index2D{0, 0};
    const auto top_left = cgfs::Index2D{-3, 2};
    const auto bottom_left = cgfs::Index2D{-3, -2};
    const auto bottom_right = cgfs::Index2D{3, -2};
    const auto top_right = cgfs::Index2D{3, 2};

    c.putPixel(center, cgfs::Color{1, 2, 3});
    c.putPixel(top_left, cgfs::Color{1, 1, 1});
    c.putPixel(bottom_left, cgfs::Color{2, 2, 2});
    c.putPixel(bottom_right, cgfs::Color{3, 3, 3});
    c.putPixel(top_right, cgfs::Color{4, 4, 4});

    const std::vector<unsigned char> expected = {
      1, 1, 1,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  4, 4, 4,
      0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,
      0, 0, 0,  0, 0, 0,  0, 0, 0,  1, 2, 3,  0, 0, 0,  0, 0, 0,  0, 0, 0,
      0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,
      2, 2, 2,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  3, 3, 3,
    };

    const auto result = std::span{c.data(), c.num_bytes()};

    CHECK(std::equal(result.begin(), result.end(), expected.begin()));
  }

  SECTION("RGBA - Single color uniform canvas")
  {
    const auto c = cgfs::Canvas{cgfs::Canvas::with_alpha_channel, {11, 7}, cgfs::Color{1, 2, 3}};
    
    const std::vector<unsigned char> expected = {
      1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,
      1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,
      1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,
      1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,
      1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,
      1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,
      1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,  1, 2, 3, 255,
    };
    
    const auto result = std::span{c.data(), c.num_bytes()};
    
    CHECK(std::equal(result.begin(), result.end(), expected.begin()));
  }
  
  SECTION("RGBA - put pixel")
  {
    auto c = cgfs::Canvas{cgfs::Canvas::with_alpha_channel, {7, 5}, cgfs::Color{0, 0, 0}};

    const auto center = cgfs::Index2D{0, 0};
    const auto top_left = cgfs::Index2D{-3, 2};
    const auto bottom_left = cgfs::Index2D{-3, -2};
    const auto bottom_right = cgfs::Index2D{3, -2};
    const auto top_right = cgfs::Index2D{3, 2};

    c.putPixel(center, cgfs::Color{1, 2, 3});
    c.putPixel(top_left, cgfs::Color{1, 1, 1});
    c.putPixel(bottom_left, cgfs::Color{2, 2, 2});
    c.putPixel(bottom_right, cgfs::Color{3, 3, 3});
    c.putPixel(top_right, cgfs::Color{4, 4, 4});

    const std::vector<unsigned char> expected = {
      1, 1, 1, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  4, 4, 4, 255,
      0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,
      0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  1, 2, 3, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,
      0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,
      2, 2, 2, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  0, 0, 0, 255,  3, 3, 3, 255,
    };

    const auto result = std::span{c.data(), c.num_bytes()};

    CHECK(std::equal(result.begin(), result.end(), expected.begin()));
  }
}
