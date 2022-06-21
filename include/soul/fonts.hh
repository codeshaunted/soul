// codeshaunted - soul
// include/soul/fonts.hh
// fonts class header file
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

#ifndef SOUL_FONTS_HH
#define SOUL_FONTS_HH

#include <cstdint>

namespace soul {

struct Font {
	uint8_t* data;
	uint32_t data_size;

	Font(uint8_t* data, uint32_t data_size) {
		this->data = data;
		this->data_size = data_size;
	}
};

class Fonts {
	public:
		static Font* getDefaultFont(); // TODO: add expected? can we assume this won't fail?
	private:
		static uint8_t default_font_data[];
		static uint32_t default_font_data_size;
		static Font default_font;
};

} // namespace soul

#endif // SOUL_FONTS_HH