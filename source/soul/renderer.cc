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
// See the License for the specific language governing permissionsand
// limitations under the License.

#include "renderer.hh"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/math.h"

#include "shaders.hh"

namespace soul {

Vertex triangle_vertices[] = {
	{-0.5f, -0.5f, 0.0f, 0xff0000ff},
	{0.5f, -0.5f, 0.0f, 0xff00ff00},
	{0.0f, 0.5f, 0.0f, 0xffff0000}
};

uint16_t triangle_indices[] = {
	0, 1, 2
};

Renderer::~Renderer() {
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
	bgfx::setViewRect(0, 0, 0, window_size.value().width, window_size.value().height);

	auto new_program = Shaders::getProgram();
	if (!new_program) return tl::unexpected(Error::UNKNOWN);

	// TODO: move this somewhere else
	bgfx::VertexLayout vertex_layout;
	vertex_layout.begin().add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float).add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true).end();
	
	bgfx::DynamicVertexBufferHandle new_vertex_buffer = bgfx::createDynamicVertexBuffer(bgfx::makeRef(triangle_vertices, sizeof(triangle_vertices)), vertex_layout);
	bgfx::DynamicIndexBufferHandle new_index_buffer = bgfx::createDynamicIndexBuffer(bgfx::makeRef(triangle_indices, sizeof(triangle_indices)));

	return new Renderer(new_window, *new_program, new_vertex_buffer, new_index_buffer);
}

Error Renderer::update() {
	const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -5.0f };
	float view_matrix[16];
	bx::mtxLookAt(view_matrix, eye, at);

	auto window_size = this->window->getSize();
	if (!window_size) return window_size.error();

	// i have no idea how projection matrices work, stole these numbers
	float projection_matrix[16];
	bx::mtxProj(projection_matrix, 60.0f, (float)window_size->width / (float)window_size->height, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

	bgfx::setViewTransform(0, view_matrix, projection_matrix);

	bgfx::setVertexBuffer(0, this->vertex_buffer);
	bgfx::setIndexBuffer(this->index_buffer);

	bgfx::submit(0, this->program);
	bgfx::frame();

	return Error::SUCCESS;
}

Renderer::Renderer(Window* new_window, bgfx::ProgramHandle new_program, bgfx::DynamicVertexBufferHandle new_vertex_buffer, bgfx::DynamicIndexBufferHandle new_index_buffer) {
	this->window = new_window;
	this->program = new_program;
	this->vertex_buffer = new_vertex_buffer;
	this->index_buffer = new_index_buffer;
}

} // namespace soul