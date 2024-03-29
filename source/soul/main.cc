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

#include "editor.hh"

using namespace soul;

int main(int argc, char** argv) {
	auto window = Window::create(800, 600, argv[0]);

	if (!window) {
		std::cerr << "failed to open window (" << (int)window.error() 
				  << ")" << std::endl;
		return 1;
	}

	auto renderer = Renderer::create(*window);

	if (!renderer) {
		std::cerr << "failed to create renderer (" << (int)renderer.error()
			<< ")" << std::endl;
		return 1;
	}

	auto gui = GUI::create(
		window.value()->getFramebufferSize()->width,
		window.value()->getFramebufferSize()->height
	);

	

	if (argc == 2) {
		Editor* ed = new Editor();

		gui.tree = ed;
		std::cout << "opening file: " << argv[1] << std::endl;
		ed->openFile(argv[1]);
	} else if (argc == 3) {
		Editor* ed = new Editor();
		std::cout << "opening file: " << argv[1] << std::endl;
		ed->openFile(argv[1]);
		ed->bg_color_abgr = 0xff221111;

		Editor* ed2 = new Editor();
		std::cout << "opening file: " << argv[2] << std::endl;
		ed2->openFile(argv[2]);
		ed2->bg_color_abgr = 0xff112211;

		gui.tree = GNonLeaf::create(true, 0.5, ed, ed2).value();
	}

	while (!window.value()->shouldClose()) {
		// haha memory churn go brrrrr
		auto cmds = gui.toCmds();

		renderer.value()->update(cmds);

		for (auto cmd : cmds) delete cmd;

		auto event = window.value()->awaitEvent();
		if (event) gui.tree->handleEvent(*event);
	}


	delete *window;
	delete *renderer;
	Window::terminateBackend();
}
