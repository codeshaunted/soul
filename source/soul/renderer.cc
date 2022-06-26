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

#include <iostream>
#include <ostream>

#include "shaders.hh"
#include "fonts.hh"

// #define LINE_HEIGHT 48

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

Renderer::~Renderer() {
	bgfx::destroy(this->program);
	bgfx::destroy(this->text_program);
	bgfx::destroy(this->text_texture_uniform);
	this->font_manager.releaseAllChars();
	bgfx::shutdown();
}

tl::expected<Renderer*, Error> Renderer::create(Window* new_window) {
	auto platform_data = new_window->getPlatformData();
	if (!platform_data) return tl::unexpected(platform_data.error());

	auto window_size = new_window->getFramebufferSize();
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
	if (!new_program) return tl::unexpected(new_program.error());

	auto text_program = Shaders::getTextProgram();
	if (!text_program) return tl::unexpected(text_program.error());

	// does nothing if they are already initialized.
	Vertex::init();
	TextVertex::init();
	
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
		uniform_handle,
		state
	);

	// bgfx::setDebug(BGFX_DEBUG_WIREFRAME);

	return r;
}

Error Renderer::update(std::vector<DrawCmd::Any*>& draw_commands) {
	auto view_matrix =	glm::mat4(1.0f);

	auto window_size = this->window->getFramebufferSize();
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

	for (auto cmd : draw_commands) {
		if (auto rectcmd = dynamic_cast<DrawCmd::Rect*>(cmd)) {
			
			bgfx::TransientVertexBuffer tvb;
			bgfx::TransientIndexBuffer tib;
			if (!bgfx::allocTransientBuffers(&tvb, Vertex::layout, 4, &tib, 6)) {
				std::cerr << "failed to allocate trasient buffers" << std::endl;
				return Error::RENDERER_ERR;
			}

			Vertex* tvbd = (Vertex*) tvb.data;

			tvbd[0].x = rectcmd->x;
			tvbd[0].y = rectcmd->y + rectcmd->height;
			tvbd[0].color = rectcmd->color;
			tvbd[1].x = rectcmd->x;
			tvbd[1].y = rectcmd->y;
			tvbd[1].color = rectcmd->color;
			tvbd[2].x = rectcmd->x + rectcmd->width;
			tvbd[2].y = rectcmd->y;
			tvbd[2].color = rectcmd->color;
			tvbd[3].x = rectcmd->x + rectcmd->width;
			tvbd[3].y = rectcmd->y + rectcmd->height;
			tvbd[3].color = rectcmd->color;

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
	
			bgfx::setState(this->bgfx_state_flags);
			bgfx::submit(0, this->program);
		} else if (auto textcmd = dynamic_cast<DrawCmd::Text*>(cmd)) {
			this->drawTextCmd(textcmd);
		}
	}

	bgfx::frame();

	return Error::SUCCESS;
}

std::optional<glm::vec2> Renderer::drawTextCmd(DrawCmd::Text* text) {
	// draw the first segment at the starting position.
	auto draw_cur = this->drawText(text->first->text, text->x, text->y, text->size, text->first->color_abgr);

	if (!draw_cur) return std::nullopt;

	auto text_cur = text->first->next;
	while (text_cur != nullptr) {
		draw_cur = this->drawText(text_cur->text, draw_cur->x, draw_cur->y, text->size, text_cur->color_abgr);
		if (!draw_cur) return std::nullopt;
		text_cur = text_cur->next;
	}
	return draw_cur;
}

std::optional<glm::vec2> Renderer::drawText(std::string_view text, float xpos, float ypos, uint16_t size, uint32_t color_abgr) {

	if (!this->font_manager.isInitialized()) {
		this->font_manager.init_default();
	}

	for (char c : text) {
		// if (!this->char_map.contains(c)) {
		// 	// for now, skip unknown characters.
		// 	continue;
		// }
		// Character& char_data = this->char_map.at(c);
		auto maybe_char_data = this->font_manager.getChar(c, size);
		if (!maybe_char_data) {
			std::cerr << "failed to draw invalid character: " << std::hex << c << " with size " << size << std::endl;
			continue;
		}
		Character* char_data = maybe_char_data.value();

		if (char_data->size.x > 0 && char_data->size.y > 0) {
			// draw this character.

			float x = xpos + char_data->bearing.x;
			// this is close enough but there is still something wrong with it.
			// as scale increases, the top of the text gets farther from the given
			// y position. If it worked properly, the top left corner of the text
			// would stay aligned with the given y position. This is also an issue
			// at scale = 1.0, but it is less noticable.
			float y = ypos + (size - char_data->bearing.y);

			float w = char_data->size.x;
			float h = char_data->size.y;

			bgfx::TransientVertexBuffer tvb;
			bgfx::TransientIndexBuffer tib;
			if (!bgfx::allocTransientBuffers(&tvb, TextVertex::layout, 4, &tib, 6)) {
				std::cerr << "failed to allocate trasient buffers" << std::endl;
				return std::nullopt;
			}

			TextVertex* tvbd = (TextVertex*) tvb.data;

			tvbd[0].x = x;
			tvbd[0].y = y + h;
			tvbd[0].color = color_abgr; 
			tvbd[0].u = 0;
			tvbd[0].v = 0x7fff;
			tvbd[1].x = x;
			tvbd[1].y = y;
			tvbd[1].color = color_abgr; 
			tvbd[1].u = 0;
			tvbd[1].v = 0;
			tvbd[2].x = x + w;
			tvbd[2].y = y;
			tvbd[2].color = color_abgr; 
			tvbd[2].u = 0x7fff;
			tvbd[2].v = 0;
			tvbd[3].x = x + w;
			tvbd[3].y = y + h;
			tvbd[3].color = color_abgr; 
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
			bgfx::setTexture(0, this->text_texture_uniform, char_data->texture.handle);
	
			bgfx::setState(this->bgfx_state_flags);
			bgfx::submit(0, this->text_program);
		}

		xpos += char_data->getAdvancePx();
	}

	return glm::vec2(xpos, ypos);
}

Renderer::Renderer(
	Window* new_window,
	bgfx::ProgramHandle new_program,
	bgfx::ProgramHandle text_program,
	bgfx::UniformHandle new_uniform_handle,
	uint64_t state
) {
	this->window = new_window;
	this->program = new_program;
	this->text_program = text_program;
	this->text_texture_uniform = new_uniform_handle;
	this->bgfx_state_flags = state;
}

} // namespace soul
