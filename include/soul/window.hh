// codeshaunted - soul
// include/soul/window.hh
// window class header file
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

#include "error.hh"

namespace soul {

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
		static tl::expected<Window*, Error> create(int width, int height, std::string title);
		tl::expected<WindowSize, Error> getSize();
		tl::expected<WindowHandle, Error> getHandle();
		tl::expected<bgfx::PlatformData, Error> getPlatformData();
	private:
		Window(GLFWwindow* new_window);
		static void glfwErrorCallback(int error, const char* description);
		static Error initializeGLFW();
		GLFWwindow* window;
		static Error glfw_error;
		static bool glfw_initialized;
};

} // namespace soul

#endif // SOUL_WINDOW_HH