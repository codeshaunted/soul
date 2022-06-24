// codeshaunted - soul
// source/soul/renderer.cc
// renderer class source file
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

#include "renderer.hh"

#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include "freetype/freetype.h"
#include "glm/fwd.hpp"
#include "tl/expected.hpp"
#include <cstdint>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#define FT_TRY(expr, fail) if((expr)) {fail;}

#include <iostream>

#include "shaders.hh"
#include "fonts.hh"

#define LINE_HEIGHT 48

namespace soul {

bgfx::VertexLayout Vertex::layout;
bool Vertex::initialized = false;


bgfx::VertexLayout TextVertex::layout;
bool TextVertex::initialized = false;

void Vertex::init() {
	if (Vertex::initialized) return;
	Vertex::layout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true, true)
		.end();
	Vertex::initialized = true;
}

void TextVertex::init() {
	if (TextVertex::initialized) return;
	TextVertex::layout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
		// .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true, true)
		.end();
	TextVertex::initialized = true;
}

Vertex triangle_vertices[] = {
	{0.f, 0.f, 0.0f, 0xff0000ff},
	{200.f, 100.f, 0.0f, 0xff00ff00},
	{150.f, 200.f, 0.0f, 0xffff0000},
	{200.f, 200.f, 0.0f, 0xffff0000},
	{0.f, 0.f, 0.0f, 0xff0000ff},
	{200.f, 100.f, 0.0f, 0xff00ff00},
	{150.f, 200.f, 0.0f, 0xffff0000},
	{200.f, 200.f, 0.0f, 0xffff0000},
	{0.f, 0.f, 0.0f, 0xff0000ff},
	{200.f, 100.f, 0.0f, 0xff00ff00},
	{150.f, 200.f, 0.0f, 0xffff0000},
	{200.f, 200.f, 0.0f, 0xffff0000},
};

uint16_t triangle_indices[] = {
	0, 2, 1, 2, 3, 1, 0, 2, 1, 2, 3, 1, 0, 2, 1, 2, 3, 1
};

Renderer::~Renderer() {
	bgfx::destroy(this->vertex_buffer);
	bgfx::destroy(this->index_buffer);
	bgfx::destroy(this->program);
	bgfx::destroy(this->text_program);
	bgfx::destroy(this->text_texture_uniform);
	for (auto & [k, v]: this->char_map) {
		if (auto h = v.getMaybeHandle()) {
			bgfx::destroy(*h);
		}
	}
	bgfx::shutdown();
}

tl::expected<Renderer*, Error> Renderer::create(Window* new_window) {
	auto platform_data = new_window->getPlatformData();
	if (!platform_data) return tl::unexpected(platform_data.error());

	auto window_size = new_window->getSize();
	if (!window_size) return tl::unexpected(window_size.error());

	bgfx::setPlatformData(platform_data.value());

	// render in this thread
	bgfx::renderFrame();

	bgfx::Init bgfx_init;
	bgfx_init.type = bgfx::RendererType::Count; // TODO: config option?
	bgfx_init.resolution.width = window_size.value().width;
	bgfx_init.resolution.height = window_size.value().height;
	bgfx_init.resolution.reset = BGFX_RESET_VSYNC; // TODO: config option?
	bool success = bgfx::init(bgfx_init);

	if (!success) return tl::unexpected(Error::UNKNOWN);

	bgfx::setViewClear(
		0,
		BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
		0x443355FF,
		1.0f,
		0
	);
	bgfx::setViewRect(0, 0, 0, window_size->width, window_size->height);

	auto new_program = Shaders::getProgram();
	if (!new_program) return tl::unexpected(Error::UNKNOWN);

	auto text_program = Shaders::getTextProgram();
	if (!text_program) return tl::unexpected(Error::UNKNOWN);

	// does nothing if they are already initialized.
	Vertex::init();
	TextVertex::init();
	
	bgfx::DynamicVertexBufferHandle new_vertex_buffer = 
		bgfx::createDynamicVertexBuffer(
			bgfx::makeRef(triangle_vertices,
			sizeof(triangle_vertices)),
			Vertex::layout
		);
	bgfx::DynamicIndexBufferHandle new_index_buffer = 
		bgfx::createDynamicIndexBuffer(
			bgfx::makeRef(
				triangle_indices, 
				sizeof(triangle_indices)
			)
		);
	
	auto uniform_handle = bgfx::createUniform("s_CharTexture", bgfx::UniformType::Sampler);

	uint64_t state = 0
					| BGFX_STATE_WRITE_RGB
					| BGFX_STATE_WRITE_A
					| BGFX_STATE_CULL_CW
					| BGFX_STATE_MSAA
					| BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
	
	auto r = new Renderer(
		new_window,
		*new_program,
		*text_program,
		new_vertex_buffer,
		new_index_buffer,
		uniform_handle,
		state
	);

	// bgfx::setDebug(BGFX_DEBUG_WIREFRAME);

	auto g = generateFontTextures(r->char_map, LINE_HEIGHT);
	if (g != Error::SUCCESS) return tl::unexpected(g);

	return r;
}

Error generateFontTextures(std::map<char, Character>& out, uint32_t line_height_px) {
	FT_Library ft;
	FT_TRY(FT_Init_FreeType(&ft), return Error::FREETYPE_ERR);

	Font* font = Fonts::getDefaultFont();
	FT_Face font_face;
	FT_TRY(FT_New_Memory_Face(ft, font->data, font->data_size, 0, &font_face), 
		std::cerr << "failed to get font" << std::endl; return Error::FREETYPE_ERR
	);

	FT_Set_Pixel_Sizes(font_face, 0, line_height_px);

	if (!bgfx::isTextureValid(0, false, 1, bgfx::TextureFormat::R8, 0)) {
		std::cerr << "texture format is invalid!" << std::endl;
		return Error::UNKNOWN;
	}

	for (uint8_t c = 0; c < 128; c++) {
		FT_TRY(FT_Load_Char(font_face, c, FT_LOAD_RENDER), return Error::FREETYPE_ERR);
		
		// generate bgfx texture from glyph bitmap
		Character result;
		
		result.texture.handle = BGFX_INVALID_HANDLE;
		result.texture.mem = nullptr;
		// if there is anything to draw (basically anything except ' '), create the texture
		if (font_face->glyph->bitmap.width) {
			if (font_face->glyph->bitmap.pitch < 0) {
				std::cerr << "negative pitch unimplemented" << std::endl;
				return Error::FREETYPE_ERR;
			}

			result.texture.mem = bgfx::copy(
				font_face->glyph->bitmap.buffer,
				font_face->glyph->bitmap.pitch * font_face->glyph->bitmap.rows
			);
			result.texture.handle = bgfx::createTexture2D(
				font_face->glyph->bitmap.width,
				font_face->glyph->bitmap.rows,
				false,
				1,
				bgfx::TextureFormat::R8,
				BGFX_SAMPLER_MAG_POINT
				| BGFX_SAMPLER_MIN_POINT
				| BGFX_SAMPLER_U_MIRROR
				| BGFX_SAMPLER_V_MIRROR, // is this right??? should this just use 0?
				result.texture.mem
			);

			if (result.texture.handle.idx == bgfx::kInvalidHandle) {
				std::cerr << "WARN: got invalid handle when generating texture for char 0x" << std::hex << c << std::endl;
				return Error::UNKNOWN;
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
		}

		result.size = glm::ivec2(font_face->glyph->bitmap.width, font_face->glyph->bitmap.rows);
		result.bearing = glm::ivec2(font_face->glyph->bitmap_left, font_face->glyph->bitmap_top);
		result.advance = font_face->glyph->advance.x;

		out.insert(std::pair<char,Character>(c, result));
	}

	// clean up freetype
	FT_Done_Face(font_face);
	FT_Done_FreeType(ft);

	return Error::SUCCESS;
}

Error Renderer::update(std::vector<std::unique_ptr<DrawCmd::Any>> draw_commands) {
	auto view_matrix =	glm::mat4(1.0f);

	auto window_size = this->window->getSize();
	if (!window_size) return window_size.error();

	// bgfx::reset(window_size->width, window_size->height);

	auto projection_matrix = 
		glm::ortho(0.f, (float)window_size->width, (float)window_size->height, 0.f);

	bgfx::setViewTransform(
		0, 
		glm::value_ptr(view_matrix), 
		glm::value_ptr(projection_matrix)
	);
	bgfx::setViewRect(0, 0, 0, window_size->width, window_size->height);

	bgfx::setViewMode(0, bgfx::ViewMode::Sequential);

	// main draw
	bgfx::setState(this->bgfx_state_flags);
	bgfx::setVertexBuffer(0, this->vertex_buffer);
	bgfx::setIndexBuffer(this->index_buffer);
	bgfx::submit(0, this->program);

	// draw text
	this->drawText("Hello, world~", 20., 10.);
	this->drawText("This text is red!", 20., 60., 0xff1111ff);
	this->drawText("this text is big!", 20., 100., 0xffdd2222, 1.5f);

	this->drawTextF("this text is colorful!", 20., 180., [](float x, float y) -> uint32_t {
		float scale = 50.f;
		uint8_t r = (uint8_t)255*sin(x/scale);
		uint8_t g = (uint8_t)255*sin(x/scale+scale);
		uint8_t b = (uint8_t)255*sin(x/scale+scale*2);
		return 0xff000000 | b << 16 | g << 8 | r;
	}, 1.0f);

	bgfx::frame();

	return Error::SUCCESS;
}


void Renderer::drawText(std::string_view text, float xpos, float ypos, uint32_t color_abgr, float scale) {
	this->drawTextF(text, xpos, ypos, [color_abgr](float x, float y) -> uint32_t {return color_abgr;}, scale);
}

// TODO: handle unicode
template<typename F>
void Renderer::drawTextF(std::string_view text, float xpos, float ypos, F color_fn, float scale) {

	for (char c : text) {
		if (!this->char_map.contains(c)) {
			// for now, skip unknown characters.
			continue;
		}
		Character& char_data = this->char_map.at(c);

		if (char_data.size.x > 0 && char_data.size.y > 0) {
			// draw this character.

			float x = xpos + char_data.bearing.x * scale;
			// this is close enough but there is still something wrong with it.
			// as scale increases, the top of the text gets farther from the given
			// y position. If it worked properly, the top left corner of the text
			// would stay aligned with the given y position. This is also an issue
			// at scale = 1.0, but it is less noticable.
			float y = ypos + (LINE_HEIGHT - char_data.bearing.y) * scale;

			float w = char_data.size.x * scale;
			float h = char_data.size.y * scale;

			bgfx::TransientVertexBuffer tvb;
			bgfx::TransientIndexBuffer tib;
			if (!bgfx::allocTransientBuffers(&tvb, TextVertex::layout, 4000, &tib, 6)) {
				std::cerr << "failed to allocate trasient buffers" << std::endl;
				return;
			}

			TextVertex* tvbd = (TextVertex*) tvb.data;

			tvbd[0].x = x;
			tvbd[0].y = y + h;
			tvbd[0].color = color_fn(x,y+h);
			tvbd[0].u = 0;
			tvbd[0].v = 0x7fff;
			tvbd[1].x = x;
			tvbd[1].y = y;
			tvbd[1].color = color_fn(x,y);
			tvbd[1].u = 0;
			tvbd[1].v = 0;
			tvbd[2].x = x + w;
			tvbd[2].y = y;
			tvbd[2].color = color_fn(x+w,y);
			tvbd[2].u = 0x7fff;
			tvbd[2].v = 0;
			tvbd[3].x = x + w;
			tvbd[3].y = y + h;
			tvbd[3].color = color_fn(x+w,y+h);
			tvbd[3].u = 0x7fff;
			tvbd[3].v = 0x7fff;

			uint16_t* tibd = (uint16_t*) tib.data;

			tibd[0] = 0;
			tibd[1] = 2;
			tibd[2] = 1;
			tibd[3] = 0;
			tibd[4] = 3;
			tibd[5] = 2;

			tib.startIndex = 0;

			bgfx::setVertexBuffer(0, &tvb);
			bgfx::setIndexBuffer(&tib);
			bgfx::setTexture(0, this->text_texture_uniform, char_data.texture.handle);
	
			bgfx::setState(this->bgfx_state_flags);
			bgfx::submit(0, this->text_program);
		}

		xpos += char_data.getAdvancePx() * scale;
	}
}

Renderer::Renderer(
	Window* new_window,
	bgfx::ProgramHandle new_program,
	bgfx::ProgramHandle text_program,
	bgfx::DynamicVertexBufferHandle new_vertex_buffer,
	bgfx::DynamicIndexBufferHandle new_index_buffer,
	bgfx::UniformHandle new_uniform_handle,
	uint64_t state
) {
	this->window = new_window;
	this->program = new_program;
	this->text_program = text_program;
	this->vertex_buffer = new_vertex_buffer;
	this->index_buffer = new_index_buffer;
	this->text_texture_uniform = new_uniform_handle;
	this->bgfx_state_flags = state;
}

} // namespace soul