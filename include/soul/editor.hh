// codeshaunted - soul
// include/soul/window.hh
// editor class header file
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

#ifndef SOUL_EDITOR_HH
#define SOUL_EDITOR_HH

#include "text_engine.hh"
#include "gui.hh"

#include <string_view>
#include <filesystem>

namespace soul {

class Editor : public GNode {
public:
	bool openFile(std::string_view path);
	bool save();
	bool saveAs();
	bool isModified() {
		return this->modified;
	}
	
	// as much as i hate initializer lists, this seems to be the only way
	// of doing this that clang accepts.
	Editor() : engine(TextEngine::create()) {
		this->cursor = {0,0};
		this->modified = false;
	}

	virtual ~Editor() override {}
	virtual void toDrawCmds(std::vector<DrawCmd::Any*>& out) override;
	virtual void handleEvent(Event e) override;

	uint32_t bg_color_abgr = 0xFF333333;
	uint32_t text_color_abgr = 0xFFEEEEEE;

	uint16_t line_height_px = 32;
	uint16_t left_margin_px = 6;

	float scroll_offset_y = 0.0;
	float scroll_offset_x = 0.0;

private:
	CurPos cursor = {0,0};
	TextEngine engine;
	bool modified = false;
	std::optional<std::filesystem::path> filePath;

	uint32_t firstVisibleLine();
	uint32_t lastVisibleLine();

	void ensureCursorVisible();

	/**
	 * just returns false if filepath isn't set
	 */
	bool saveInternal();
};

}
#endif // SOUL_EDITOR_HH

