// codeshaunted - soul
// include/soul/font_manager.hh
// font manager class header file
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

#include "bgfx/bgfx.h"
#include "glm/glm.hpp"

#include "ft2build.h"
#include "tl/expected.hpp"
#include FT_FREETYPE_H

#include "error.hh"
#include "fonts.hh"

#include <optional>
#include <map>


namespace soul {

struct Character {
	struct {
		bgfx::TextureHandle handle; // handle of the glyph texture
		const bgfx::Memory* mem;
		bgfx::TextureInfo info;
	} texture;
	glm::ivec2 size; // size of glyph
	glm::ivec2 bearing; // Offset from baseline to top left of glyph
	unsigned int advance; // offset to advance to next glyph

	int getAdvancePx() {
		// advance value is in 1/64th pixels
		return this->advance >> 6;
	}

	inline std::optional<bgfx::TextureHandle>getMaybeHandle() {
		if (this->size.x > 0 && this->size.y > 0) {
			return this->texture.handle;
		} else {
			return std::nullopt;
		}
	}
};

#define DEFAULT_SIZE 48

class FontMgr {
	public:

		Error init(Font* font);
		Error init_default() {
			return this->init(Fonts::getDefaultFont());
		}
		bool isInitialized() {return this->initialized;}

		/**
		 * get data about a given character of a given size.
		 * This function will generate Character data if `c`
		 * is not already loaded, which includes creating a
		 * bgfx texture.
		 *
		 * @returns a non-owning pointer if the character can be loaded
		 */
		tl::expected<Character*, Error> getChar(char c, uint16_t size = DEFAULT_SIZE);

		// unload a char, including destroying its texture handle.
		void releaseChar(char c, uint16_t size);

		void releaseAllChars();

	~FontMgr();
	FontMgr() {
		this->initialized = false;
		this->ft = {0};
		this->font_face = {0};
	}

	private:
		bool initialized;
		FT_Library ft;
		FT_Face font_face;

		// maps from char and size to a Character struct.
		std::map<std::pair<char, uint16_t>, Character> char_map;
};

}

//		