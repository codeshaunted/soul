// codeshaunted - soul
// source/soul/renderer.cc
// renderer class source file
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

namespace soul {

DrawCmd::Text* DrawCmd::Text::create(std::string text, float x, float y, uint32_t color, float scale) {
	auto ret = new Text();
	ret->x = x;
	ret->y = y;
	ret->scale = scale;
	ret->first = new TextSegment(text, color);
	return ret;
}

void DrawCmd::Text::append(std::string text, uint32_t color) {
	if (this->first) {

		auto cur = this->first;

		while (cur->next != nullptr) {
			cur = cur->next;
		}
		// cur is now a pointer to the last segment.
		cur->next = new TextSegment(text, color);
	} else {
		// there is no first segment (this shouldn't be possible, but the case
		// should be handled anyway)
		this->first = new TextSegment(text, color);
	}
}

DrawCmd::Text::~Text() {
	if (this->first) {
		delete this->first;
	}
}

DrawCmd::TextSegment::~TextSegment() {
	if (this->next)
		delete this->next;
}

}
