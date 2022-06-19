// codeshaunted - soul
// tests/test_gui.cc
// gui test source file
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

#include "gui.hh"

#include <cstdint>
#include <iostream>

using namespace soul;

TEST_CASE("simple GUI tree") {
  auto gui = GUI::create();
  auto red = new GLeaf(0xff0000ff);
  auto blue = new GLeaf(0xffff0000);
  auto green = new GLeaf(0xff00ff00);
  auto node1 = GNonLeaf::create(true, 0.5f, red, green).value();
  auto node2 = GNonLeaf::create(false, 0.5, node1, blue).value();
  gui.tree = node2;

  auto tris = gui.toCmds(100, 100);
  
  // constexpr Rect expected[] = {
  //   {0.f, 0.f, 0.f, 0xff0000ff},
  //   {50.f, 0.f, 0.f, 0xff0000ff},
  //   {0.f, 50.f, 0.f, 0xff0000ff},
  // };

  // CHECK(tris.first.size() == 12);
  // for (int i = 0; i < 12; i++) {
  //   auto& e = expected[i];
  //   auto& t = tris.first[i];
  //   CHECK(e.x == t.x);
  //   CHECK(e.y == t.y);
  //   CHECK(e.z == t.z);
  //   CHECK(e.color == t.color);
  // }

  // constexpr uint16_t expected_i[] = {
  //   2, 1, 0, 2, 3, 1, 6, 5, 4, 6, 7, 5, 10, 9, 8, 10, 11, 9
  // };

  // CHECK(tris.second.size() == 18);
  // for (int i = 0; i < 18; i++) {
  //   CHECK(expected_i[i] == tris.second[i]);
  // }
  for (auto& e : tris) {
    e->debugLog();
  }
}