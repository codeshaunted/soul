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
#include "utils.hh"

#include "tinyfiledialogs.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <optional>

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
	this->engine.insertStr({0,0}, data);

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

}
