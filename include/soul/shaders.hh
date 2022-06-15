// codeshaunted - soul
// include/soul/shaders.hh
// shaders class header file
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

#ifndef SOUL_SHADERS_HH
#define SOUL_SHADERS_HH

#include "tl/expected.hpp"
#include "bgfx/bgfx.h"

#include "error.hh"

namespace soul {

class Shaders {
	public:
		static tl::expected<bgfx::ShaderHandle, Error> getVertexShader();
		static tl::expected<bgfx::ShaderHandle, Error> getFragmentShader();
		static tl::expected<bgfx::ProgramHandle, Error> getProgram();
		static tl::expected<bgfx::ShaderHandle, Error> getTextVertexShader();
		static tl::expected<bgfx::ShaderHandle, Error> getTextFragmentShader();
		static tl::expected<bgfx::ProgramHandle, Error> getTextProgram();
	private:
#ifdef SOUL_IS_WINDOWS
		static uint8_t vertex_dx9[];
		static uint32_t vertex_dx9_size;
		static uint8_t fragment_dx9[];
		static uint32_t fragment_dx9_size;
		static uint8_t vertex_dx11[];
		static uint32_t vertex_dx11_size;
		static uint8_t fragment_dx11[];
		static uint32_t fragment_dx11_size;
		static uint8_t vertex_text_dx9[];
		static uint32_t vertex_text_dx9_size;
		static uint8_t fragment_text_dx9[];
		static uint32_t fragment_text_dx9_size;
		static uint8_t vertex_text_dx11[];
		static uint32_t vertex_text_dx11_size;
		static uint8_t fragment_text_dx11[];
		static uint32_t fragment_text_dx11_size;
#endif // SOUL_IS_WINDOWS
#if defined(SOUL_IS_WINDOWS) || defined(SOUL_IS_LINUX)
		static uint8_t vertex_opengl[];
		static uint32_t vertex_opengl_size;
		static uint8_t fragment_opengl[];
		static uint32_t fragment_opengl_size;
		static uint8_t vertex_spirv[];
		static uint32_t vertex_spirv_size;
		static uint8_t fragment_spirv[];
		static uint32_t fragment_spirv_size;
		static uint8_t vertex_text_opengl[];
		static uint32_t vertex_text_opengl_size;
		static uint8_t fragment_text_opengl[];
		static uint32_t fragment_text_opengl_size;
		static uint8_t vertex_text_spirv[];
		static uint32_t vertex_text_spirv_size;
		static uint8_t fragment_text_spirv[];
		static uint32_t fragment_text_spirv_size;
#endif // SOUL_IS_WINDOWS || SOUL_IS_LINUX
#ifdef SOUL_IS_MACOS
		static uint8_t vertex_metal[];
		static uint32_t vertex_metal_size;
		static uint8_t fragment_metal[];
		static uint32_t fragment_metal_size;
		static uint8_t vertex_text_metal[];
		static uint32_t vertex_text_metal_size;
		static uint8_t fragment_text_metal[];
		static uint32_t fragment_text_metal_size;
#endif // SOUL_IS_MACOS
};

} // namespace soul

#endif // SOUL_SHADERS_HH