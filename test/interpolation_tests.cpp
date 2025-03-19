#include <catch2/catch_test_macros.hpp>

#include "interpolation.h"

TEST_CASE("Horizontal-ish lines")
{
  SECTION("Given intensity")
  {
    const auto a = cgfs::Index2D{0, 0};
    const auto b = cgfs::Index2D{3, 0};
    const auto result = cgfs::interpolate(a, b);
    REQUIRE(result.size() == 4);
    REQUIRE(result[0] == cgfs::Index2D{0, 0});
    REQUIRE(result[1] == cgfs::Index2D{1, 0});
    REQUIRE(result[2] == cgfs::Index2D{2, 0});
    REQUIRE(result[3] == cgfs::Index2D{3, 0});

    SECTION("Order doesn't matter")
    {
      const auto result_ba = cgfs::interpolate(b, a);
      REQUIRE(result_ba.size() == result.size());
      REQUIRE(std::equal(result_ba.begin(), result_ba.end(), result.begin()));
    }
  }

  SECTION("Given intensity")
  {
    const auto a = cgfs::Index2D{-1, -1};
    const auto b = cgfs::Index2D{10, 9};
    const auto result = cgfs::interpolate(a, b);
    REQUIRE(result.front() == a);
    REQUIRE(result.back() == b);

    SECTION("Order doesn't matter")
    {
      const auto result_ba = cgfs::interpolate(b, a);
      REQUIRE(result_ba.size() == result.size());
      REQUIRE(std::equal(result_ba.begin(), result_ba.end(), result.begin()));
    }

  }
}

TEST_CASE("Vertical-ish lines")
{
  SECTION("Given intensity")
  {
    const auto a = cgfs::Index2D{0, 0};
    const auto b = cgfs::Index2D{0, 3};
    const auto result = cgfs::interpolate(a, b);
    REQUIRE(result.size() == 4);
    REQUIRE(result[0] == cgfs::Index2D{0, 0});
    REQUIRE(result[1] == cgfs::Index2D{0, 1});
    REQUIRE(result[2] == cgfs::Index2D{0, 2});
    REQUIRE(result[3] == cgfs::Index2D{0, 3});

    SECTION("Order doesn't matter")
    {
      const auto result_ba = cgfs::interpolate(b, a);
      REQUIRE(result_ba.size() == result.size());
      REQUIRE(std::equal(result_ba.begin(), result_ba.end(), result.begin()));
    }
  }

  SECTION("Given intensity")
  {
    const auto a = cgfs::Index2D{-1, -1};
    const auto b = cgfs::Index2D{9, 10};
    const auto result = cgfs::interpolate(a, b);
    REQUIRE(result.front() == a);
    REQUIRE(result.back() == b);
    //

    SECTION("Order doesn't matter")
    {
      const auto result_ba = cgfs::interpolate(b, a);
      REQUIRE(result_ba.size() == result.size());
      REQUIRE(std::equal(result_ba.begin(), result_ba.end(), result.begin()));
    }
  }
}