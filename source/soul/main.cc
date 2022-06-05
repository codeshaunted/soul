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

using namespace soul;

int main(int argc, char** argv) {
	auto window = Window::create(800, 600, "test");

	if (window) {
		std::cout << "successfully created window!!!" << std::endl;
		auto handle = window->getHandle();

		if (handle) {
			std::cout << *handle << std::endl;
		}
	}
	else {
		std::cout << (int)window.error() << std::endl;
	}

	for (;;);
}