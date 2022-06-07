#include "doctest.h"

#include "window.hh"

using namespace soul;

TEST_CASE("basic window functions") {
  auto window = Window::create(800, 600, "test window");

  CHECK(window.has_value());

  SUBCASE("get platform data") {
    auto pd = window.value()->getPlatformData();
    CHECK(pd.has_value());
    CHECK(pd.value().nwh != NULL);
  }
}