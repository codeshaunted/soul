// codeshaunted - soul
// source/soul/gui.cc
// gui class source file
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

#include "gui.hh"
#include "renderer.hh"

#include <cstdint>
#include <memory>
#include <optional>

namespace soul {

GUI GUI::create() {
	return GUI();
}

GUI GUI::create(unsigned int window_width, unsigned int window_height) {
	GUI ret;
	ret.setDimensions(window_width, window_height);
	return ret;
}

void GUI::setDimensions(unsigned int window_width, unsigned int window_height) {
	this->root_bounding_box = Rect {
		0, 0, (float)window_width, (float)window_height, 0xff000000
	};
}

uint16_t addVert(std::vector<Vertex>& vec, Vertex to_add) {
	// TODO: don't add duplicate verts?
	uint16_t idx = vec.size();
	vec.push_back(to_add);
	return idx;
}

std::optional<GNonLeaf*> GNonLeaf::create(bool horizontal, float split, GNode *first, GNode *second) {
	if (
		split >= 1 || 
		split <= 0 ||
		!first ||
		!second
	) return std::nullopt;
	return new GNonLeaf(horizontal, split, first, second);
}

GNonLeaf::GNonLeaf(bool split_horizontal, float split, GNode* first, GNode* second) {
	this->horizontal = split_horizontal;
	this->split_point = split;
	this->first = first;
	this->second = second;
}

void GNonLeaf::handleEvent(Event e) {
	this->first->handleEvent(e);
	this->second->handleEvent(e);
}

std::vector<DrawCmd::Any*> GUI::toCmds() {
	// make sure dimensions are correct
	this->tree->updateBoundingBox(this->root_bounding_box);

	std::vector<DrawCmd::Any*> ret;
	this->tree->toDrawCmds(ret);
	return ret;
}

void GLeaf::toDrawCmds(std::vector<DrawCmd::Any*> &out) {
	// current.color = node_leaf.color;
	out.push_back(new DrawCmd::Rect(
		this->bounding_box.x,
		this->bounding_box.y,
		this->bounding_box.width,
		this->bounding_box.height,
		this->color
	));
}

void GNonLeaf::updateBoundingBox(Rect new_box) {
	// calculate Rects for each child, and recurse them.
	Rect r1, r2;
	if (this->horizontal) {
		r1.width = new_box.width;
		r2.width = new_box.width;
		r1.x = new_box.x;
		r2.x = new_box.x;
		r1.y = new_box.y;
		float height1 = this->split_point * new_box.height;
		r1.height = height1;
		r2.height = new_box.height - height1;
		r2.y = new_box.y + height1;
	} else {
		r1.height = new_box.height;
		r2.height = new_box.height;
		r1.y = new_box.y;
		r2.y = new_box.y;
		r1.x = new_box.x;
		float width1 = this->split_point * new_box.height;
		r1.width = width1;
		r2.width = new_box.width - width1;
		r2.x = new_box.x + width1;
	}
	this->first->updateBoundingBox(r1);
	this->second->updateBoundingBox(r2);
}

void GNonLeaf::toDrawCmds(std::vector<DrawCmd::Any*> &out) {
	this->first->toDrawCmds(out);
	this->second->toDrawCmds(out);
}

}