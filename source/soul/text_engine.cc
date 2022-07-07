// codeshaunted - soul
// source/soul/text_engine.cc
// text engine class source file
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

#include "text_engine.hh"

#include <iostream>
#include <sstream>
#include <algorithm>

namespace soul {

Line::~Line() {
	// nothing?
}

Line Line::create() {
	return Line();
}

Line Line::from(std::string _text) {
	return Line(_text);
}

Line::Line() {
	this->text = std::string();
}

Line::Line(std::string _text) {
	this->text = _text;
}

TextEngine TextEngine::create() {
	return TextEngine(std::nullopt);
}

tl::expected<std::vector<Line>, std::string> splitIntoLines(std::string_view text) {
	std::vector<Line> lines;
	std::string currentLine;
	
	for (unsigned int i = 0; i < text.length(); i++) {
		char c = text[i];
		if (!iscntrl(c)) {
			currentLine.append(1, c);
		} else if (c == '\n') {
			lines.push_back(Line::from(currentLine));
			currentLine = std::string();
		} else if (c == '\r' and text[i+1] == '\n') {
			lines.push_back(Line::from(currentLine));
			currentLine = std::string();
			i += 1; // skip \n
		} else {
			// encountered an unsupported control character!

			std::stringstream message;
			message << "unsupported control character " << std::hex << c;
			return tl::unexpected(message.str());
		}
	}
	if (currentLine.length() > 0) {
		lines.push_back(Line::from(currentLine));
	}

	return lines;
}

tl::expected<TextEngine, std::string> TextEngine::from(std::string_view text) {
	// initial_text might be multiple lines, in which case it needs to be split.

	return splitIntoLines(text).map([](auto v){
		return TextEngine(v);
	});
}

TextEngine::TextEngine(std::optional<std::vector<Line>> initial) {
	if (!initial) {
		// start with one empty line if nothing is provided
		this->lines.push_back(Line::create());
	} else {
		this->lines = initial.value();
	}
}

TextEngine::~TextEngine() {}

std::string TextEngine::toString() {
	std::string out;
	for (auto line : this->lines) {
		out.append(line.text);
		out.append(1, '\n');
	}
	return out;
}

unsigned int TextEngine::numLines() {return this->lines.size();}

std::optional<std::vector<std::string_view>> TextEngine::getLines(unsigned int from, unsigned int to) {
	auto num = this->lines.size();
	if (to < from || from >= num || to >= num) {
		return std::nullopt;
	}
	std::vector<std::string_view> ret;
	for (unsigned int i = from; i <= to; i++) {
		ret.push_back(this->lines[i].text);
	}
	return ret;
}

tl::expected<Line*, Error> TextEngine::getLine(unsigned int num) {
	if (num < 0 || num >= this->lines.size()) {
		return tl::unexpected(Error::IDX_OUT_OF_RANGE);
	}
	
	return &this->lines[num];
}

bool TextEngine::canInsert(CurPos pos) {
	if (pos.first == this->lines.size()) return pos.second == 0;
	return pos.first < this->lines.size() &&
		   pos.second <= this->lines[pos.first].text.length();
}

std::optional<CurPos> TextEngine::newLineAfter(unsigned int line) {
	if (line > this->lines.size()) {
		line = this->lines.size();
	}
	this->lines.insert(this->lines.begin() + line + 1, Line::create());
	return CurPos{line + 1, 0};
}

std::optional<CurPos> TextEngine::insert(CurPos pos, char to_insert) {
	if (!this->canInsert(pos)) {
		std::cerr << "cannot insert at " << pos.first << ":" << pos.second << "!" << std::endl;
		return std::nullopt;
	}

	// first check if this is past the end of the file at a new line
	if (pos.first == this->lines.size()) {
		this->lines.push_back(Line::create());
	}

	if (to_insert == '\n') { // handle newline insertion
		std::string& line_ref = this->lines[pos.first].text;
		if (pos.second == line_ref.length()) {
			// if we're at the end of a line, all we need to do is insert an empty line after this.
			return this->newLineAfter(pos.first);
		} else {
			std::string after = line_ref.substr(pos.second);
			// erase everything after the cursor...
			line_ref.erase(pos.second, std::string::npos);
			// and put in on the next line.
			this->lines.insert(this->lines.begin() + pos.first + 1, Line::from(after));
		}
		// cursor will now be on the start of the next line.
		return CurPos{pos.first + 1, 0};
	} else if (iscntrl(to_insert)) { // don't allow any other control characters
		std::cerr << "cannot insert invalid character" << std::hex << to_insert << std::endl;
		return std::nullopt;
	} else { // if the character is anything else, insert it normally.
		std::string& r = this->lines[pos.first].text;
		r.insert(r.begin() + pos.second, to_insert);
		return CurPos{pos.first, pos.second + 1};  
	}
}



std::optional<CurPos> TextEngine::insertStr(CurPos pos, std::string_view str) {
	if (!this->canInsert(pos)) {
		std::cerr << "cannot insert at " << pos.first << ":" << pos.second << "!" << std::endl;
		return std::nullopt;
	}
	auto new_lines = splitIntoLines(str);
	if (!new_lines) return std::nullopt;

	// This needs to change
	//    line:{start}{end}
	// and turn it into
	//    line: {start}{input[0]}
	//  line+n: {input[n]]}
	//    last: {input[last]}{end}
	// so its kinda complicated and confusing :(

	// first check if this is past the end of the file at a new line
	if (pos.first == this->lines.size()) {
		this->lines.push_back(Line::create());
	}

	std::string& current_line = this->lines[pos.first].text;

	// optimization: if it is only one line, just insert
	if (new_lines->size() == 1) {
		auto& t = (*new_lines)[0].text;
		current_line.insert(pos.second, t);
		return CurPos{pos.first, pos.second + t.length()};
	}

	auto new_start = current_line.substr(0, pos.second);
	new_start.append((*new_lines)[0].text);

	auto rest_of_line = current_line.substr(pos.second, current_line.length());
	std::string& last = new_lines->rbegin()[0].text;
	auto len_before_concat = last.length();
	// is this better than .append? no idea lmao
	last.insert(last.end(), rest_of_line.begin(), rest_of_line.end());

	// modify list
	(this->lines.begin() + pos.first)[0].text = new_start;

	current_line = new_start;
	this->lines.insert(this->lines.begin() + pos.first + 1, new_lines->begin() + 1, new_lines->end());

	return CurPos{pos.first + new_lines->size() - 1, len_before_concat};
}

/**
 * print a vec<string> as
 * [
 *   1:blah
 *   2:blah blah
 *   ...
 * ]
 */
void dPrintVec(std::vector<std::string>& v) {
	std::cout << "[" << std::endl;
	for(size_t i = 0; i < v.size(); i++) {
		std::cout << i << ":" << v[i] << std::endl;
	}
	std::cout << "]" << std::endl;
}
/**
 * see dPrintVec. unwraps Line.text.
 */
void dPrintLines(std::vector<Line>& v) {
	std::cout << "[" << std::endl;
	for(size_t i = 0; i < v.size(); i++) {
		std::cout << i << ":" << v[i].text << std::endl;
	}
	std::cout << "]" << std::endl;
}

std::optional<CurPos> TextEngine::deleteChar(CurPos pos) {
	if (!this->canInsert(pos)) return std::nullopt;
	
	// if the cursor is on the first char of a line, append this line to the previous line.
	if (pos.second == 0) {
		if (pos.first == 0) {
			// if we are at the first character of the first line, do nothing.
			return CurPos{0,0};
		}
		// append to previous
		auto& prev_text_r = this->lines[pos.first - 1].text;
		prev_text_r.append(this->lines[pos.first].text);
		// delete line
		this->lines.erase(this->lines.begin() + pos.first);
		return CurPos{pos.first - 1, prev_text_r.length()};
	} else {
		this->lines[pos.first].text.erase(pos.second - 1, 1);
		return CurPos{pos.first, pos.second - 1};
	}
}

std::optional<CurPos> TextEngine::deleteRange(unsigned int line, unsigned int start, unsigned int end) {
	if (
		!this->canInsert({line, start}) ||
		!this->canInsert({line,end}) ||
		end <= start
	) {
		return std::nullopt;
	}

	std::string& line_ref = this->lines[line].text;
	line_ref.erase(line_ref.begin() + start, line_ref.begin() + end + 1);
	return CurPos{line, start};
}

std::optional<CurPos> TextEngine::deleteRange(CurPos from, CurPos to) {
	if (from.first == to.first) {
		return this->deleteRange(from.first, from.second, to.second);
	}
	
	std::string& start_line = this->lines[from.first].text;
	std::string& end_line = this->lines[to.first].text;

	auto new_start = start_line.substr(0, from.second);
	auto new_end = end_line.substr(to.second);

	this->lines[from.first].text = new_start + new_end;
	this->lines.erase(this->lines.begin() + from.first + 1, this->lines.begin() + to.first + 1);

	return CurPos{from.first, from.second};
}

CurPos TextEngine::clear() {
	this->lines.clear();
	return {0, 0};
}

} // namespace soul
