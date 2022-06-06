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

using namespace soul;

int main(int argc, char** argv) {
	// example textengine usage:
	{
		auto engine = TextEngine::from("line 3\nline 4\nline 5").value();

		std::cout << "before:" << std::endl << engine.to_string() << std::endl;

		engine.insert_str(0, 5, "1\nline 2\nline ");

		std::cout << "after insert: " << std::endl << engine.to_string() << std::endl;

		engine.delete_range(1, 2, 2, 3); // "lie 3"

		std::cout << "after delete_range: " << std::endl << engine.to_string() << std::endl;

		engine.delete_char(0,1);
		
		std::cout << "after delete_char: " << std::endl << engine.to_string() << std::endl;

		engine.delete_range(3,0,2);

		std::cout << "after delete_range: " << std::endl << engine.to_string() << std::endl;

		engine.insert(2,0,'\n');

		std::cout << "after newline insert: " << std::endl << engine.to_string() << std::endl;

		engine.insert(3,2,'\n');

		std::cout << "after newline insert 2: " << std::endl << engine.to_string() << std::endl;

		auto lines_view = *engine.get_lines(0, engine.num_lines()-1);
		for (auto line : lines_view) {
			std::cout << line.length() << " ";
		}
		std::cout << std::endl;
	}

	auto window = Window::create(800, 600, "test");

	if (window) {
		std::cout << "successfully created window!!!" << std::endl;
		auto handle = window->getHandle();

		if (handle) {
			std::cout << *handle << std::endl;
		} else {
			std::cout << "unable to get native window handle! error: " << std::hex << (int)handle.error() << std::endl;
		}
	}
	else {
		std::cout << (int)window.error() << std::endl;
	}

	for (;;);
}