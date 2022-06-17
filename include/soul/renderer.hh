// codeshaunted - soul
// include/soul/renderer.hh
// renderer class header file
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

#ifndef SOUL_RENDERER_HH
#define SOUL_RENDERER_HH

#include "bgfx/bgfx.h"
#include "glm/glm.hpp"
#include "tl/expected.hpp"

#include <cstdint>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string_view>

#include "window.hh" 
#include "error.hh"
#include <map>
#include <vector>
#include <cstddef>

namespace soul {

struct Character {
	struct {
		bgfx::TextureHandle handle; // handle of the glyph texture
		const bgfx::Memory* mem; // i think we need to free this later...
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

struct Vertex {
	float x;
	float y;
	float z;
	uint32_t color;

	static bgfx::VertexLayout layout;
	static bool initialized;
	static void init();
};
static_assert(offsetof(Vertex, x) == 0, "offset of vertex x must be 0");
static_assert(offsetof(Vertex, y) == 4, "offset of vertex y must be 4");
static_assert(offsetof(Vertex, z) == 8, "offset of vertex z must be 8");
static_assert(offsetof(Vertex, color) == 12, "offset of vertex z must be 12");

struct TextVertex {
	float x;
	float y;
	float z;
	uint32_t color;
	int16_t u;
	int16_t v;

	static bgfx::VertexLayout layout;
	static bool initialized;
	static void init();
};
static_assert(offsetof(TextVertex, x) == 0, "offset of vertex x must be 0");
static_assert(offsetof(TextVertex, y) == 4, "offset of vertex y must be 4");
static_assert(offsetof(TextVertex, z) == 8, "offset of vertex z must be 8");
static_assert(offsetof(TextVertex, color) == 12, "offset of vertex z must be 12");
static_assert(offsetof(TextVertex, u) == 16, "offset of vertex u must be 16");
static_assert(offsetof(TextVertex, v) == 18, "offset of vertex v must be 18");

class Renderer {
	public:
		~Renderer();
		static tl::expected<Renderer*, Error> create(Window* window);
		void setVertexAndIndexBuffers(std::vector<Vertex>& verts, std::vector<uint16_t> indices);
		Error update();
	private:
		Renderer(
			Window* new_window,
			bgfx::ProgramHandle new_program,
			bgfx::ProgramHandle text_program,
			bgfx::DynamicVertexBufferHandle new_vertex_buffer,
			bgfx::DynamicIndexBufferHandle new_index_buffer,
			bgfx::UniformHandle new_uniform_handle,
			uint64_t state
		);
		template<typename F>
		void drawTextF(std::string_view text, float xpos, float ypos, F color_fn, float scale = 1.0f);
		void drawText(std::string_view text, float xpos, float ypos, uint32_t color_abgr = 0xffffffff, float scale = 1.0f);
		Window* window;
		bgfx::ProgramHandle program;
		bgfx::ProgramHandle text_program;
		bgfx::DynamicVertexBufferHandle vertex_buffer;
		bgfx::DynamicIndexBufferHandle index_buffer;
		std::map<char, Character> char_map;
		uint64_t bgfx_state_flags;
		bgfx::UniformHandle text_texture_uniform;
};

Error generate_font_textures(std::map<char, Character>& out, const char* name, uint32_t line_height_px);

} // namespace soul

#endif // SOUL_RENDERER_HH