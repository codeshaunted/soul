// codeshaunted - soul
// source/soul/main.cc
// soul entry point file
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

#include <iostream>

#include "GLFW/glfw3.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include "window.hh"
#include "renderer.hh"
#include "text_engine.hh"
#include "shaders.hh"

using namespace soul;

int main(int argc, char** argv) {
	auto window = Window::create(800, 600, "test");

	if (!window) {
		std::cerr << "failed to open window (" << (int)window.error() 
				  << ")" << std::endl;
		return 1;
	}

	std::cerr << "successfully created window!!!" << std::endl;

	auto renderer = Renderer::create(*window);

	if (!renderer) {
		std::cerr << "failed to create renderer (" << (int)renderer.error()
			<< ")" << std::endl;
		return 1;
	}

	while (!window.value()->shouldClose()) {
		renderer.value()->update();
		auto event = window.value()->awaitEvent();
	}

	delete *window;
	delete *renderer;
	Window::terminateBackend();
}