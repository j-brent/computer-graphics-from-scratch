#include <catch2/catch_test_macros.hpp>

#include "light.h"

TEST_CASE("Ambient light")
{
  SECTION("Given intensity")
  {
    cgfs::AmbientLight al{0.5};
    REQUIRE(al.intensity() == 0.5);
  }
}

TEST_CASE("Directional light")
{
  SECTION("Given intensity")
  {
    cgfs::DirectionalLight dl{0.2f, {1, 4, 4}};

    REQUIRE(dl.intensity({}, {1, 4, 4}) == 0.2f);
    REQUIRE(dl.intensity({}, {0, 1, -1}) == 0.0f);
    REQUIRE(dl.intensity({}, {-1, -4, -4}) == 0.0f);
  }
}

TEST_CASE("Point light")
{
  SECTION("Given intensity")
  {
    cgfs::PointLight pl{0.6f, {1, 1, 1}};

    REQUIRE(pl.intensity({0,0,0}, {1, 1, 1}) == 0.6f);
    REQUIRE(pl.intensity({2,2,2}, {-1, -1, -1}) == 0.6f);
    REQUIRE(pl.intensity({0,0,0}, {-1, -1, -1}) == 0.0f);

    std::vector<cgfs::Light> lights{pl};
    const auto& rpl = lights[0];
    REQUIRE(rpl.intensity({0,0,0}, {1, 1, 1}) == 0.6f);
    REQUIRE(rpl.intensity({2,2,2}, {-1, -1, -1}) == 0.6f);
    REQUIRE(rpl.intensity({0,0,0}, {-1, -1, -1}) == 0.0f);

  }
}

SCENARIO("I saw the light", "[light]"){
  GIVEN("Hank Williams"){
    WHEN("He sings"){
      THEN("He has seen the light"){
        REQUIRE(1 == 1);
        CHECK(true);
      }
    }
  }
}