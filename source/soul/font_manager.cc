// codeshaunted - soul
// source/soul/font_manager.cc
// font manager class source file
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

#include "font_manager.hh"
#include "bgfx/bgfx.h"
#include "error.hh"
#include "fonts.hh"
#include "freetype/freetype.h"
#include "freetype/fttypes.h"
#include "tl/expected.hpp"

#include <iostream>

#define FT_TRY(expr, fail) if((expr)) {fail;}

namespace soul {

Error FontMgr::init(Font *font) {
	this->initialized = false;
	FT_TRY(FT_Init_FreeType(&this->ft), return Error::FREETYPE_ERR);

	FT_TRY(FT_New_Memory_Face(this->ft, font->data, font->data_size, 0, &this->font_face), 
		std::cerr << "failed to load font" << std::endl; return Error::FREETYPE_ERR
	);

	if (this->char_map.size() > 0) {
		this->releaseAllChars();
	}

	if (!bgfx::isTextureValid(0, false, 1, bgfx::TextureFormat::R8, 0)) {
		std::cerr << "texture format is invalid!" << std::endl;
		return Error::UNKNOWN;
	}

	this->initialized = true;

	return Error::SUCCESS;
}

void FontMgr::releaseChar(char c, uint16_t size) {
	if (!this->char_map.contains({c, size})) return;

	auto& data = this->char_map.at({c, size});
	if (auto h = data.getMaybeHandle()) {
		bgfx::destroy(*h);
	}

	this->char_map.erase({c, size});
}

void FontMgr::releaseAllChars() {
	for (auto& [k, v] : this->char_map) {
		if (auto h = v.getMaybeHandle()) {
			bgfx::destroy(*h);
		}
	}

	this->char_map.clear();
}

FontMgr::~FontMgr() {
	if (!this->initialized) return;

	FT_Done_Face(this->font_face);
	FT_Done_FreeType(this->ft);
}

// magic stackoverflow function to get error string
const char* getErrorMessage(FT_Error err){
	#undef FTERRORS_H_
	#define FT_ERRORDEF( e, v, s )  case e: return s;
	#define FT_ERROR_START_LIST     switch (err) {
	#define FT_ERROR_END_LIST       }
	#include FT_ERRORS_H
	return "(Unknown error)";
}

tl::expected<Character*, Error> FontMgr::getChar(char c, uint16_t size) {
	if (!this->initialized) return tl::unexpected(Error::UNINITAILIZED);

	if (this->char_map.contains({c, size})) {
		return &this->char_map.at({c, size});
	}

#define FF this->font_face // for brevity

	// this character hasn't been generated yet.
	
	// it seems like this should only need to be called once when the size changes, but
	// for some reason that doesn't seem to work...
	FT_TRY(FT_Set_Pixel_Sizes(FF, 0, size), 
		std::cerr << "failed to set pixel sizes" << std::endl;
		return tl::unexpected(Error::FREETYPE_ERR)
	);

	FT_Error e;
	FT_TRY(e = FT_Load_Char(FF, c, FT_LOAD_RENDER), 
		std::cerr << "failed to load char " << std::hex << c << ": " << getErrorMessage(e) <<std::endl;
		std::cerr << "size is " << size << std::endl;
		return tl::unexpected(Error::FREETYPE_ERR);
	);

	// generate bgfx texture from glyph bitmap.
	Character result;

	result.texture.handle = BGFX_INVALID_HANDLE;
	result.texture.mem = nullptr;

	// If there is anything to draw (ie, this isn't a whitespace character), create
	// the texture.
	if (FF->glyph->bitmap.width) {
		if (FF->glyph->bitmap.pitch < 0) {
			std::cerr << "negative pitch bitmap unimplemented" << std::endl;
			return tl::unexpected(Error::FREETYPE_ERR);
		}

		result.texture.mem = bgfx::copy(
			FF->glyph->bitmap.buffer,
			FF->glyph->bitmap.pitch * FF->glyph->bitmap.rows
		);

		result.texture.handle = bgfx::createTexture2D(
			FF->glyph->bitmap.width,
			FF->glyph->bitmap.rows,
			false,
			1,
			bgfx::TextureFormat::R8,
			BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_U_MIRROR
			| BGFX_SAMPLER_V_MIRROR, // is this right??? should this just use 0?
			result.texture.mem
		);

		if (!bgfx::isValid(result.texture.handle)) {
			std::cerr << "got invalid handle when generating texture for char 0x" << std::hex << c << std::endl;
			return tl::unexpected(Error::UNKNOWN);
		}

		bgfx::calcTextureSize(
			result.texture.info,
			font_face->glyph->bitmap.width,
			font_face->glyph->bitmap.rows,
			1, // is this the right value???? hopefully it doesn't matter...
			false,
			false,
			1,
			bgfx::TextureFormat::R8
		);
	} // if FF->glyph->bitmap.width


	result.size = glm::ivec2(font_face->glyph->bitmap.width, font_face->glyph->bitmap.rows);
	result.bearing = glm::ivec2(font_face->glyph->bitmap_left, font_face->glyph->bitmap_top);
	result.advance = font_face->glyph->advance.x;

	this->char_map.insert({{c, size}, result});

	return &this->char_map.at({c, size});
#undef FF
}

}