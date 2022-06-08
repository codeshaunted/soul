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

using namespace soul;

TEST_CASE("basic TextEngine text manipulation") {
	auto engine = TextEngine::from("line 3\nline 4\nline 5").value();

  CHECK(engine.toString() == "line 3\nline 4\nline 5\n");

	engine.insertStr(0, 5, "1\nline 2\nline ");

  CHECK(engine.toString() == "line 1\nline 2\nline 3\nline 4\nline 5\n");

	engine.deleteRange(1, 2, 2, 3);

  CHECK(engine.toString() == "line 1\nlie 3\nline 4\nline 5\n");

	engine.deleteChar(0,1);

  auto l = engine.getLine(0);
  CHECK(l.has_value());
  CHECK(l.value()->text == "lne 1");
  CHECK(engine.toString() == "lne 1\nlie 3\nline 4\nline 5\n");

	engine.deleteRange(3,0,2);

  CHECK(engine.toString() == "lne 1\nlie 3\nline 4\ne 5\n");

	engine.insert(2,0,'\n');

  CHECK(engine.toString() == "lne 1\nlie 3\n\nline 4\ne 5\n");

	engine.insert(3,2,'\n');

  CHECK(engine.toString() == "lne 1\nlie 3\n\nli\nne 4\ne 5\n");
}