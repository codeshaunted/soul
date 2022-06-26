#include "bgfx/bgfx.h"
#include "doctest.h"

#include "font_manager.hh"

#include <cstddef>
#include <iostream>

using namespace soul;

TEST_CASE("font manager") {
  bgfx::Init i;
  i.type = bgfx::RendererType::Noop;
  bgfx::init(i);
  FontMgr fm;
  fm.init_default();

  auto c = fm.getChar('a', 64);
  CHECK(c);
  CHECK(c.value()->getMaybeHandle());

  for (auto i = 'a'; i <= 'z'; i++) {
    fm.getChar(i);
  }

  fm.releaseChar('a', 64);
  fm.releaseAllChars();
  bgfx::shutdown();
}