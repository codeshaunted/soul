// codeshaunted - soul
// source/soul/main.cc
// entry point file
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

#include <iostream>

#include "window.hh"
#include "textengine.hh"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

using namespace soul;

void testTextEngine();

int main(int argc, char** argv) {
	testTextEngine();

	auto window = Window::create(800, 600, "test");

	if (!window) {
		std::cout << "failed to open window (" << (int)window.error() 
				  << ")" << std::endl;
		return 1;
	}

	std::cout << "successfully created window!!!" << std::endl;
	auto pd = window.value()->getPlatformData();

	if (!pd) {
		std::cout
			<< "unable to get native platform data! error: "
			<< std::hex << (int)pd.error() << std::endl;
		return 1;
	}
	
	// just render an empty window for now.

	bgfx::setPlatformData(pd.value());

	// 
	bgfx::renderFrame();

	bgfx::Init bgfxInit;

	// TODO: should window return a PlatformData, or an Init?

	bgfxInit.type = bgfx::RendererType::Count;
	bgfxInit.resolution.width = 800;
	bgfxInit.resolution.height = 600;
	bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
	auto code =  bgfx::init(bgfxInit);
	if (!code) {
		std::cerr << "failed to init bgfx!" << std::endl;
		return 1;
	}
	
	bgfx::setViewClear(
		0,
		BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
		0x443355FF,
		1.0f,
		0
	);
	bgfx::setViewRect(0, 0, 0, 800, 600);

	while (true) {
		bgfx::touch(0);
		bgfx::frame();
	}
	delete *window;
	bgfx::shutdown();
	Window::deinit_backend();
}

void testTextEngine() {
	// example textengine usage:
	auto engine = TextEngine::from("line 3\nline 4\nline 5").value();

	// std::cout << "before:" << std::endl << engine.to_string() << std::endl;

	engine.insert_str(0, 5, "1\nline 2\nline ");

	engine.delete_range(1, 2, 2, 3); // "lie 3"

	engine.delete_char(0,1);

	engine.delete_range(3,0,2);

	engine.insert(2,0,'\n');

	engine.insert(3,2,'\n');

	std::cout << "after: " << std::endl << engine.to_string() << std::endl << "lengths: ";

	auto lines_view = *engine.get_lines(0, engine.num_lines()-1);
	for (auto line : lines_view) {
		std::cout << line.length() << " ";
	}
	std::cout << std::endl;
}