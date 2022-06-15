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
  CHECK(FT_Load_Char(face, '{', FT_LOAD_RENDER) == 0);
  std::cout << "w " << face->glyph->bitmap.width << std::endl
            << "h " << face->glyph->bitmap.rows << std::endl
            << "p " << face->glyph->bitmap.pitch << std::endl;
  for (int y = face->glyph->bitmap.rows-1; y >= 0; y--) {
    for (int x = 0; x < face->glyph->bitmap.width; x++) {
      uint8_t c = face->glyph->bitmap.buffer[y * face->glyph->bitmap.width + x];
      constexpr const char* palette[] = {" ", ".", "-", "~", "+", "="};
      auto i = c/51;
      // std::cout << std::hex << i;
      std::cout << palette[i];
    }
    std::cout << std::endl;
  }
}