// codeshaunted - soul
// tests/test_text_engine.cc
// text engine test source file
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
// See the License for the specific language governing permissionsand
// limitations under the License.

#include "doctest.h"

#include "text_engine.hh"

#include <iostream>

using namespace soul;

TEST_CASE("basic TextEngine text manipulation") {
	auto engine = TextEngine::from(
    "line 1\n"
    "line 2\n"
    "line 3\n"
    "line 4\n"
    "line 5"
    // "line 5"
  ).value();

  CHECK(engine.toString() == "line 1\nline 2\nline 3\nline 4\nline 5\n");

  SUBCASE("insertStr") {
    auto p = engine.insertStr({4, 1}, "ine 5\nl");

    CHECK(p.has_value());
    CHECK(p.value() == CurPos{5,1});
    CHECK(engine.toString() == "line 1\nline 2\nline 3\nline 4\nline 5\nline 5\n");
  }

  SUBCASE("deleteRange") {
    auto p = engine.deleteRange({1, 2}, {2, 3});

    CHECK(p.has_value());
    CHECK(*p == CurPos{1,2});
    CHECK(engine.toString() == "line 1\nlie 3\nline 4\nline 5\n");
  }

  SUBCASE("deleteChar") {
    auto p = engine.deleteChar({0,1});

    CHECK(p.has_value());
    CHECK(*p == CurPos{0,0});
    auto l = engine.getLine(0);
    CHECK(l.has_value());
    CHECK(l.value()->text == "ine 1");
    CHECK(engine.toString() == "ine 1\nline 2\nline 3\nline 4\nline 5\n");
  }

	SUBCASE("deleteRange") {
    auto p = engine.deleteRange(3,0,2);

    CHECK(p.has_value());
    CHECK(*p == CurPos{3,0});
    CHECK(engine.toString() == "line 1\nline 2\nline 3\ne 4\nline 5\n");
  }

	SUBCASE("insert nl at start") {
    auto p = engine.insert({2,0},'\n');

    CHECK(p.has_value());
    CHECK(*p == CurPos{3,0});
    CHECK(engine.toString() == "line 1\nline 2\n\nline 3\nline 4\nline 5\n");
  }

  SUBCASE("insert nl in middle") {
    auto p = engine.insert({3,2},'\n');

    CHECK(p.has_value());
    CHECK(*p == CurPos{4,0});
    CHECK(engine.toString() == "line 1\nline 2\nline 3\nli\nne 4\nline 5\n");
  }
}