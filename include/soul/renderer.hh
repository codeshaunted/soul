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

#include "tl/expected.hpp"

#include "window.hh"
#include "error.hh"
#include <vector>

namespace soul {

struct Vertex {
	float x;
	float y;
	float z;
	uint32_t color;
};

class Renderer {
	public:
		~Renderer();
		static tl::expected<Renderer*, Error> create(Window* window);
		void setVertexAndIndexBuffers(std::vector<Vertex>& verts, std::vector<uint16_t> indices);
		Error update();
	private:
		Renderer(Window* new_window, bgfx::ProgramHandle new_program, bgfx::DynamicVertexBufferHandle new_vertex_buffer, bgfx::DynamicIndexBufferHandle new_index_buffer);
		Window* window;
		bgfx::ProgramHandle program;
		bgfx::DynamicVertexBufferHandle vertex_buffer;
		bgfx::DynamicIndexBufferHandle index_buffer;
};

} // namespace soul

#endif // SOUL_RENDERER_HH