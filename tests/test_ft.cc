#include "doctest.h"
#include "freetype/freetype.h"
#include "renderer.hh"
#include <iostream>

TEST_CASE("simple freetype usage") {
  FT_Library ft;
  CHECK(FT_Init_FreeType(&ft)==0);
  FT_Face face;
  CHECK(FT_New_Face(ft, "CascadiaCode.ttf", 0, &face) == 0);
  FT_Set_Pixel_Sizes(face, 0, 48);
  CHECK(FT_Load_Char(face, 'c', FT_LOAD_RENDER) == 0);
  std::cout << "w " << face->glyph->bitmap.width << std::endl
            << "h " << face->glyph->bitmap.rows << std::endl;
}