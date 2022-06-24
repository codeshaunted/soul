// codeshaunted - soul
// source/soul/window.cc
// window class source file
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

#include "window.hh"

#include "events.hh"

#include "GLFW/glfw3.h"
#include "bgfx/platform.h"
#if defined(SOUL_IS_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(SOUL_IS_LINUX)
// TODO: handle wayland
// #define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_X11
#elif defined(SOUL_IS_MACOS)
#define GLFW_EXPOSE_NATIVE_COCOA
#endif // SOUL_IS_WINDOWS, SOUL_IS_LINUX
#include "GLFW/glfw3native.h"

#include <iostream>

namespace soul {

#define SOUL_GLFW_CATCH_ERROR() Window::glfw_error = Error::SUCCESS;
#define SOUL_GLFW_GET_ERROR() Window::glfw_error

Window::~Window() {
	if (this->window) {
		glfwDestroyWindow(this->window);
		this->window = nullptr;
	}
}

// TODO: should this be triggered automatically? perhaps when the last window
//       is destroyed?
void Window::terminateBackend() {
	if (Window::glfw_initialized) {
		glfwTerminate();
		Window::glfw_initialized = false;
	}
}

tl::expected<Window*, Error> Window::create(int width, int height, std::string title) {
	if (!Window::glfw_initialized) {
		Error initialize_error = Window::initializeGLFW();

		if (initialize_error != Error::SUCCESS) return tl::unexpected(initialize_error);
	}

	SOUL_GLFW_CATCH_ERROR();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	
	glfwSetKeyCallback(window, Window::glfwKeyCallback);
	glfwSetCharCallback(window, Window::glfwCharacterCallback);
	glfwSetCursorPosCallback(window, Window::glfwCursorPositionCallback);
	glfwSetCursorEnterCallback(window, Window::glfwCursorEnterLeaveCallback);
	glfwSetMouseButtonCallback(window, Window::glfwMouseButtonCallback);
	glfwSetScrollCallback(window, Window::glfwScrollCallback);
	glfwSetWindowSizeCallback(window, Window::glfwWindowSizeCallback);

	if (window && SOUL_GLFW_GET_ERROR() == Error::SUCCESS) return new Window(window);
	else return tl::unexpected(SOUL_GLFW_GET_ERROR());
}

tl::expected<WindowSize, Error> Window::getSize() {
	WindowSize size;

	SOUL_GLFW_CATCH_ERROR();
	glfwGetWindowSize(this->window, &size.width, &size.height);

	if (SOUL_GLFW_GET_ERROR() == Error::SUCCESS) return size;
	else return tl::unexpected(SOUL_GLFW_GET_ERROR());
}

tl::expected<WindowHandle, Error> Window::getHandle() {
	WindowHandle handle;

	SOUL_GLFW_CATCH_ERROR();
#if defined(SOUL_IS_WINDOWS)
	handle = glfwGetWin32Window(this->window);
#elif defined(SOUL_IS_LINUX)
	// handle = glfwGetWaylandWindow(this->window);
	handle = (void*)glfwGetX11Window(this->window);
#elif defined(SOUL_IS_MACOS)
	handle = (void*)glfwGetCocoaWindow(this->window);
#endif // SOUL_IS_WINDOWS, SOUL_IS_LINUX

	if (handle == NULL) return tl::unexpected(Error::UNKNOWN);
	if (SOUL_GLFW_GET_ERROR() == Error::SUCCESS) return handle;
	else return tl::unexpected(SOUL_GLFW_GET_ERROR());
}

tl::expected<bgfx::PlatformData, Error> Window::getPlatformData() {
	bgfx::PlatformData ret;

	auto maybe_nwh = this->getHandle();
	if (!maybe_nwh) return tl::unexpected(maybe_nwh.error());

	ret.nwh = *maybe_nwh;

#ifdef SOUL_IS_LINUX
	// using x11, we also need to provide the display.
	SOUL_GLFW_CATCH_ERROR();
	auto ndt = glfwGetX11Display();

	if (SOUL_GLFW_GET_ERROR() != Error::SUCCESS)
		return tl::unexpected(SOUL_GLFW_GET_ERROR());
	
	ret.ndt = ndt;
#endif // SOUL_IS_LINUX

	return ret;
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(this->window);
}

void Window::setShouldClose(bool val) {
	glfwSetWindowShouldClose(this->window, val);
}

Window::Window(GLFWwindow* new_window) {
	this->window = new_window;
}

void Window::glfwErrorCallback(int error, const char* description) {
	Window::glfw_error = Error(error);
}

std::optional<Event> Window::awaitEvent() {
	latestEvent = std::nullopt;
	windowAwaitingEvent = this->window;
	glfwWaitEvents();
	windowAwaitingEvent = std::nullopt;
	return latestEvent;
}

#define CHECK_WINDOW_AWAIT(window) \
	if (!windowAwaitingEvent.has_value() || *windowAwaitingEvent != (window)) { \
		latestEvent = std::nullopt; \
		return; \
	}

void Window::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	CHECK_WINDOW_AWAIT(window);
	// std::cout << "got key event to window " << std::hex << window << std::endl;
	latestEvent = KeyEvent {key, scancode, keyActionFromGLFW(action), KeyboardModifiers::fromGLFW(mods)};
}

void Window::glfwCharacterCallback(GLFWwindow* window, unsigned int codepoint) {
	CHECK_WINDOW_AWAIT(window);

	if (codepoint != static_cast<uint8_t>(codepoint)) {
		std::cerr << "unsupported unicode codepoint: " << std::hex << codepoint << std::endl;
		latestEvent = std::nullopt;
	} else {
		// std::cout << "got character event: '" << static_cast<char>(codepoint) << "'" << std::endl;
		latestEvent = TextEvent {static_cast<uint8_t>(codepoint)};
	}
}

void Window::glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	CHECK_WINDOW_AWAIT(window);

	// std::cout << "got cursor position event: (" << xpos << ", " << ypos << ")" << std::endl;
	latestEvent = MouseCursorPosEvent{xpos, ypos};
}

void Window::glfwCursorEnterLeaveCallback(GLFWwindow* window, int entered) {
	CHECK_WINDOW_AWAIT(window);

	// std::cout << (entered == GLFW_TRUE ? "got cursor enter event" : "got cursor leave event") << std::endl;
	latestEvent = MouseCursorEnterExitEvent{entered == GLFW_TRUE};
}

void Window::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	CHECK_WINDOW_AWAIT(window);

	auto m = KeyboardModifiers::fromGLFW(mods);
	auto a = keyActionFromGLFW(action);
	auto b = mouseButtonFromGLFW(button);
	// std::cout << "got mouse button event: button " << button << " action " << action << "mods" << mods << std::endl;
	latestEvent = MouseButtonEvent{b, a, m};
}

void Window::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	CHECK_WINDOW_AWAIT(window);

	// std::cout << "got scroll event: (" << xoffset << ", " << yoffset << ")" << std::endl;
	latestEvent = ScrollEvent{xoffset, yoffset};
}

void Window::glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
	CHECK_WINDOW_AWAIT(window)

	latestEvent = WindowResizeEvent{(unsigned int)width, (unsigned int)height};
}

#undef CHECK_WINDOW_AWAIT

Error Window::initializeGLFW() {
	if (!Window::glfw_initialized) {
		glfwSetErrorCallback(Window::glfwErrorCallback);

		SOUL_GLFW_CATCH_ERROR();
		Window::glfw_initialized = glfwInit();

		return SOUL_GLFW_GET_ERROR();
	}

	return Error::SUCCESS;
}

Error Window::glfw_error = Error::SUCCESS;
bool Window::glfw_initialized = false;

std::optional<GLFWwindow*> Window::windowAwaitingEvent;
std::optional<Event> Window::latestEvent;

#undef SOUL_GLFW_CATCH_ERROR
#undef SOUL_GLFW_GET_ERROR

} // namespace soul