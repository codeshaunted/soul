// codeshaunted - soul
// source/soul/window.cc
// window class source file
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

#include "window.hh"

#if defined(SOUL_IS_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(SOUL_IS_LINUX)
// #define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_X11
#endif // SOUL_IS_WINDOWS, SOUL_IS_LINUX
#include "GLFW/glfw3native.h"

namespace soul {

#define SOUL_GLFW_CATCH_ERROR() Window::glfw_error = WindowError::SUCCESS;
#define SOUL_GLFW_GET_ERROR() Window::glfw_error

Window::~Window() {
	if (this->window) {
		glfwDestroyWindow(this->window);
		this->window = nullptr;
	}
}

WindowError Window::initializeGLFW_() {
	if (!Window::glfw_initialized) {
		glfwSetErrorCallback(glfwErrorCallback_);

		SOUL_GLFW_CATCH_ERROR();
		Window::glfw_initialized = glfwInit();

		return SOUL_GLFW_GET_ERROR();
	}

	return WindowError::SUCCESS;
}

tl::expected<Window, WindowError> Window::create(int width, int height, std::string title) {
	if (!Window::glfw_initialized) {
		WindowError initialize_error = initializeGLFW_();

		if (initialize_error != WindowError::SUCCESS) return tl::unexpected(initialize_error);
	}

	SOUL_GLFW_CATCH_ERROR();
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (window && SOUL_GLFW_GET_ERROR() == WindowError::SUCCESS) return Window(window);
	else return tl::unexpected(SOUL_GLFW_GET_ERROR());
}

tl::expected<WindowSize, WindowError> Window::getSize() {
	WindowSize size;

	SOUL_GLFW_CATCH_ERROR();
	glfwGetWindowSize(this->window, &size.height, &size.width);

	if (SOUL_GLFW_GET_ERROR() == WindowError::SUCCESS) return size;
	else return tl::unexpected(SOUL_GLFW_GET_ERROR());
}

tl::expected<WindowHandle, WindowError> Window::getHandle() {
	WindowHandle handle;

	SOUL_GLFW_CATCH_ERROR();
#if defined(SOUL_IS_WINDOWS)
	handle = glfwGetWin32Window(this->window);
#elif defined(SOUL_IS_LINUX)
	// handle = glfwGetWaylandWindow(this->window);
	handle = (void*)glfwGetX11Window(this->window);
#endif // SOUL_IS_WINDOWS, SOUL_IS_LINUX

	if (SOUL_GLFW_GET_ERROR() == WindowError::SUCCESS) return handle;
	else return tl::unexpected(SOUL_GLFW_GET_ERROR());
}

Window::Window(GLFWwindow* new_window) {
	this->window = new_window;
}

void Window::glfwErrorCallback_(int error, const char* description) {
	Window::glfw_error = WindowError(error);
}

WindowError Window::glfw_error = WindowError::SUCCESS;
bool Window::glfw_initialized = false;

#undef SOUL_GLFW_CATCH_ERROR
#undef SOUL_GLFW_GET_ERROR

} // namespace soul