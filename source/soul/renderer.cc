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
// #include "bx/math.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/type_ptr.hpp>

#define FT_TRY(expr, fail) if((expr)) {fail;}

#include <iostream>

#include "shaders.hh"

namespace soul {

// Vertex triangle_vertices[] = {
// 	{-0.5f, -0.5f, 0.0f, 0xff0000ff},
// 	{0.5f, -0.5f, 0.0f, 0xff00ff00},
// 	{0.0f, 0.5f, 0.0f, 0xffff0000}
// };

bgfx::VertexLayout Vertex::layout;
bool Vertex::initialized = false;


bgfx::VertexLayout TextVertex::layout;
bool TextVertex::initialized = false;

void Vertex::init() {
	if (Vertex::initialized) return;
	Vertex::layout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
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
		.end();
	TextVertex::initialized = true;
}

Error generate_font_textures(std::map<char, Character>& out, const char* name);

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
	bgfx::shutdown();
}

tl::expected<Renderer*, Error> Renderer::create(Window* new_window) {
	auto platform_data = new_window->getPlatformData();
	if (!platform_data) return tl::unexpected(platform_data.error());

	auto window_size = new_window->getSize();
	if (!window_size) return tl::unexpected(window_size.error());

	bgfx::setPlatformData(platform_data.value());

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

	auto text_program = Shaders::getProgram();
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
	
	auto r = new Renderer(new_window, *new_program, *text_program, new_vertex_buffer, new_index_buffer);

	// ==== freetype setup ====

	auto g = generate_font_textures(r->char_map, "CascadiaCode.ttf");
	if (g != Error::SUCCESS) return tl::unexpected(g);

	return r;
}

Error generate_font_textures(std::map<char, Character>& out, const char* name) {
	FT_Library ft;
	FT_TRY(FT_Init_FreeType(&ft), return Error::FREETYPE_ERR);

	FT_Face font_face;
	FT_TRY(FT_New_Face(ft, "CascadiaCode.ttf", 0, &font_face), return Error::FREETYPE_ERR);

	FT_Set_Pixel_Sizes(font_face, 0, 48);

	// FT_TRY(FT_Load_Char(r->font_face, 'X', FT_LOAD_RENDER), return tl::unexpected(Error::FREETYPE_ERR));

	if (!bgfx::isTextureValid(0, false, 1, bgfx::TextureFormat::R8U, 0)) {
		std::cerr << "texture format is invalid!" << std::endl;
		return Error::UNKNOWN;
	}

	for (uint8_t c = 0; c < 128; c++) {
		FT_TRY(FT_Load_Char(font_face, c, FT_LOAD_RENDER), return Error::FREETYPE_ERR);
		// generate bgfx texture from glyph bitmap
		Character result;

		// bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
		if (font_face->glyph->bitmap.pitch < 0) {
			std::cerr << "negative pitch unimplemented" << std::endl;
			return Error::FREETYPE_ERR;
		}
		//const bgfx::Memory* 
		result.texture.mem = bgfx::copy(
			font_face->glyph->bitmap.buffer,
			font_face->glyph->bitmap.pitch * font_face->glyph->bitmap.rows
		);
		result.texture.handle = bgfx::createTexture2D(
			font_face->glyph->bitmap.width,
			font_face->glyph->bitmap.rows,
			false,
			1,
			bgfx::TextureFormat::R8U,
			BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP, // is this right??? should this just use 0?
			result.texture.mem
		);

		if (!bgfx::isValid(result.texture.handle)) {
			std::cerr << "texture created was invalid!" << std::endl;
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
			bgfx::TextureFormat::R8U
		);

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


void Renderer::setVertexAndIndexBuffers(std::vector<Vertex> &verts, std::vector<uint16_t> indices) {
	auto vs = verts.size() * sizeof(Vertex);
	auto is = indices.size() * sizeof(uint16_t);
	auto vmem = bgfx::copy(verts.data(), vs);
	auto imem = bgfx::copy(indices.data(), is);
	bgfx::update(this->vertex_buffer, 0, vmem);
	bgfx::update(this->index_buffer, 0, imem);
}

Error Renderer::update() {
	auto view_matrix =	glm::mat4(1.0f);

	auto window_size = this->window->getSize();
	if (!window_size) return window_size.error();

	bgfx::reset(window_size->width, window_size->height);

	auto projection_matrix = 
		glm::ortho(0.f, (float)window_size->width, (float)window_size->height, 0.f);

	bgfx::setViewTransform(
		0, 
		glm::value_ptr(view_matrix), 
		glm::value_ptr(projection_matrix)
	);
	bgfx::setViewRect(0, 0, 0, window_size->width, window_size->height);

	bgfx::setVertexBuffer(0, this->vertex_buffer);
	bgfx::setIndexBuffer(this->index_buffer);

	bgfx::submit(0, this->program);
	bgfx::frame();

	return Error::SUCCESS;
}

Renderer::Renderer(
	Window* new_window,
	bgfx::ProgramHandle new_program,
	bgfx::ProgramHandle text_program,
	bgfx::DynamicVertexBufferHandle new_vertex_buffer,
	bgfx::DynamicIndexBufferHandle new_index_buffer
) {
	this->window = new_window;
	this->program = new_program;
	this->text_program = text_program;
	this->vertex_buffer = new_vertex_buffer;
	this->index_buffer = new_index_buffer;
}

} // namespace soul