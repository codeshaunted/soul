// codeshaunted - soul
// tests/test_window.cc
// window test source file
// Copyright 2022 the soul team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org / licenses / LICENSE - 2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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