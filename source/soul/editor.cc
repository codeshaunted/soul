// codeshaunted - soul
// source/soul/gui.cc
// editor class source file
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

#include "editor.hh"
#include "events.hh"
#include "renderer.hh"
#include "utils.hh"

#include "tinyfiledialogs.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <optional>

#define SCROLL_MULTIPLIER -10.0

namespace soul {

bool Editor::openFile(std::string_view path) {

	if (this->modified) {
		int choice = tinyfd_messageBox(
			"Unsaved Content",
			"You appear to have unsaved changes. Save before opening?",
			"yesnocancel",
			"question",
			2
		);

		if (choice == 0) {
			// cancel
			return false;
		} else if (choice == 1) {
			// yes
			this->save();
		} else {
			// choice == 2 ("no") -> do nothing
		}
	}
	
	this->filePath = std::nullopt;

	auto maybeData = utils::readFilePath({path});
	if (!maybeData) return false;
	auto data = *maybeData;
	
	this->filePath = {path};
	this->engine.clear();
	if (!this->engine.insertStr({0,0}, data)) {
		// this keeps failing >:( need more textengine tests
		std::cerr << "internal error: failed to insert text when opening file" << std::endl;
		return false;
	}

	this->cursor = {0,0};
	this->modified = false;

	return true;
}

bool Editor::saveInternal() {
	// this shouldn't be the case, but check anyway.
	if (!this->filePath) return false;

	std::ofstream outstream(this->filePath->c_str(), std::ofstream::binary);

	for (auto line : this->engine.lines) {
		outstream.write(line.text.c_str(), line.text.length());
		outstream.write("\n", 1);
	}

	if (outstream.bad() || outstream.fail()) {
		std::cerr << "ERR: outstream failed in saveInternal" << std::endl;
		outstream.close();
		return false;
	} else {
		outstream.close();
		return true;
	}
}

bool Editor::save() {
	if (this->filePath) {
		return this->saveInternal();
	} else {
		// no path set, so we fall back to save as
		return this->saveAs();
	}
}

bool Editor::saveAs() {
	char const* const defaultPath =
		this->filePath ? this->filePath->c_str() : "";
	
	auto result = tinyfd_saveFileDialog(
		"Save file",
		defaultPath,
		0,
		NULL,
		0
	);

	if (!result) return false;

	this->filePath = {result};
	return this->saveInternal();
}

void Editor::toDrawCmds(std::vector<DrawCmd::Any *> &out, Rect bounding_box) {
	// add background rect
	out.push_back(new DrawCmd::Rect(
		bounding_box.x,
		bounding_box.y,
		bounding_box.width,
		bounding_box.height,
		this->bg_color_abgr
	));

	// next, generate draw commands for each line that is visible.

	// Caclulate which lines are visible
	auto height_px = bounding_box.height;
	// any line between this->scroll and this->scroll + height is visible.
	// to get from a pixel scroll offset to a line, divide by this->line_height_px

	unsigned first_line = this->scroll_offset_y / this->line_height_px;
	unsigned last_line = (this->scroll_offset_y + height_px) / this->line_height_px;

	auto lines_in_file = this->engine.numLines();

	for (unsigned i = first_line; i <= last_line && i < lines_in_file; i++) {
		float from_top_px = i * this->line_height_px;
		auto from_top_of_view_px = from_top_px - this->scroll_offset_y;
		auto screenspace_y_px = bounding_box.y + from_top_of_view_px;
		auto screenspace_x_px = bounding_box.x + this->left_margin_px - this->scroll_offset_x;
		auto cmd = DrawCmd::Text::create(
			// TODO: perhaps avoid copying the string here? this code could be optimized a lot.
			this->engine.getLine(i).value()->text,
			screenspace_x_px,
			screenspace_y_px,
			this->line_height_px
		);
		out.push_back(cmd);
	}
}

void Editor::handleEvent(Event e) {
	// yep this is kinda yucky but the inner code isn't too bad at least?
	std::visit([this](auto&& r){
		using T = std::decay_t<decltype(r)>;
		if constexpr (std::is_same_v<T, ScrollEvent>) {
			this->scroll_offset_y += SCROLL_MULTIPLIER * r.y;
			if (this->scroll_offset_y < 0) {
				// it would be nice to have some kind of animation for this, but that needs
				// a way to request a rerender which we don't have yet. this works for now.
				this->scroll_offset_y = 0;
			} else if (this->scroll_offset_y > (this->engine.numLines() + 1) * this->line_height_px) {
				this->scroll_offset_y = (this->engine.numLines() + 1) * this->line_height_px;
			}

			this->scroll_offset_x += SCROLL_MULTIPLIER * r.x;
			if (this->scroll_offset_x < 0) {
				this->scroll_offset_x = 0;
			}
		} else {

		}
	}, e);
}

}
