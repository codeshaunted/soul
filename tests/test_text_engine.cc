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