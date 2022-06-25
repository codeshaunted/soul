// codeshaunted - soul
// source/soul/debug.cc
// debug source file
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

#include "renderer.hh"

#include <iostream>

using namespace soul;

void DrawCmd::Any::debugLog() {
	std::cout << "[Any]" << std::endl;
}

void DrawCmd::Rect::debugLog() {
	std::cout
		<< "[Rect (" << this->x << ", " << this->y << ") w "
		<< this->width << " h " << this->height << " c "
		<< std::hex << this->color << "]" << std::endl;
}

void DrawCmd::Text::debugLog() {
	std::cout
		<< "[Text (" << this->x << "," << this->y << ") s" << this->scale;
	if (!this->first) {
		std::cout << " {empty}";
	} else {
		auto cur = this->first;
		while (cur) {
			std::cout << std::endl << "  "
				<< std::hex << cur->color_abgr << " \""
				<< cur->text << "\"";
			cur = cur->next;
		}
	}
	std::cout << std::endl << "]" << std::endl;
}
