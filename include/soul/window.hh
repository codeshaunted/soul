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

#include <string>
#include <optional>

#include "tl/expected.hpp"
#include "GLFW/glfw3.h"
#include "bgfx/bgfx.h"

#include "error.hh"

#include "events.hh"

namespace soul {

struct WindowSize {
	int width;
	int height;
};

typedef void* WindowHandle;

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
		bool shouldClose();
		void setShouldClose(bool val);
		// This wraps around glfwWaitEvents and also handles events.
		std::optional<Event> awaitEvent();
	private:
		Window(GLFWwindow* new_window);
		static void glfwErrorCallback(int error, const char* description);
		
		static std::optional<GLFWwindow*> windowAwaitingEvent;
		static std::optional<Event> latestEvent;

		static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void glfwCharacterCallback(GLFWwindow* window, unsigned int codepoint);
		static void glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void glfwCursorEnterLeaveCallback(GLFWwindow* window, int entered);
		static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		static Error initializeGLFW();
		GLFWwindow* window;
		static Error glfw_error;
		static bool glfw_initialized;
};

} // namespace soul

#endif // SOUL_WINDOW_HH