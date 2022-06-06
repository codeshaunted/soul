// codeshaunted - soul
// include/soul/window.hh
// window class header file
// Copyright 2021 codeshaunted
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

#ifndef SOUL_WINDOW_HH
#define SOUL_WINDOW_HH

#if defined(SOUL_IS_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(SOUL_IS_LINUX)
// #include <wayland-client-protocol.h>
#endif // SOUL_IS_WINDOWS, SOUL_IS_LINUX

#include <string>

#include "tl/expected.hpp"
#include "GLFW/glfw3.h"
#include "bgfx/bgfx.h"

namespace soul {

// undef these macros and replace them with our enum
#undef GLFW_NOT_INITIALIZED
#undef GLFW_NO_CURRENT_CONTEXT
#undef GLFW_INVALID_ENUM
#undef GLFW_INVALID_VALUE
#undef GLFW_OUT_OF_MEMORY
#undef GLFW_API_UNAVAILABLE
#undef GLFW_VERSION_UNAVAILABLE
#undef GLFW_PLATFORM_ERROR
#undef GLFW_FORMAT_UNAVAILABLE
enum class WindowError {
	SUCCESS = 0,
	UNKNOWN = 1,
	GLFW_NOT_INITIALIZED = 0x00010001,
	GLFW_NO_CURRENT_CONTEXT = 0x00010002,
	GLFW_INVALID_ENUM = 0x00010003,
	GLFW_INVALID_VALUE = 0x00010004,
	GLFW_OUT_OF_MEMORY = 0x00010005,
	GLFW_API_UNAVAILABLE = 0x00010006,
	GLFW_VERSION_UNAVAILABLE = 0x00010007,
	GLFW_PLATFORM_ERROR = 0x00010008,
	GLFW_FORMAT_UNAVAILABLE = 0x00010009
};

struct WindowSize {
	int width;
	int height;
};

#if defined(SOUL_IS_WINDOWS)
typedef HWND WindowHandle;
#elif defined(SOUL_IS_LINUX)
// TODO: is this the best type to use? 
typedef void* WindowHandle;
#endif // SOUL_IS_WINDOWS, SOUL_IS_LINUX

class Window {
	public:
		~Window();
		static void terminateBackend();
		/**
		 * creates and allocates a window (should be deleted).
		 */
		static tl::expected<Window*, WindowError> create(int width, int height, std::string title);
		tl::expected<WindowSize, WindowError> getSize();
		tl::expected<WindowHandle, WindowError> getHandle();
		tl::expected<bgfx::PlatformData, WindowError> getPlatformData();
	private:
		Window(GLFWwindow* window);
		static void glfwErrorCallback(int error, const char* description);
		static WindowError initializeGLFW();
		GLFWwindow* window;
		static WindowError glfw_error;
		static bool glfw_initialized;
};

} // namespace soul

#endif // SOUL_WINDOW_HH