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
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <memory>

#include "GLFW/glfw3.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include "window.hh"
#include "renderer.hh"
#include "text_engine.hh"
#include "shaders.hh"
#include "gui.hh"

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

	auto gui = GUI::create();
	// gui.tree = std::make_unique<GLeaf>(0xff00ff00);
	auto leaf1 = new GLeaf(0xff2d3338);
	auto leaf2 = new GLeaf(0xff525f68);
	auto leaf3 = new GLeaf(0xff66869b);
	auto node1 = GNonLeaf::create(true, 0.5, leaf1, leaf2).value();
	auto root = GNonLeaf::create(false, 0.3, leaf3, node1).value();
	gui.tree = root;

	auto tris = gui.toGeometry(window.value()->getSize()->width, window.value()->getSize()->height);

	renderer.value()->setVertexAndIndexBuffers(tris.first, tris.second);

	while (!window.value()->shouldClose()) {
		renderer.value()->update();
		auto event = window.value()->awaitEvent();
	}

	delete *window;
	delete *renderer;
	Window::terminateBackend();
}

#undef FT_TRY