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
// See the License for the specific language governing permissionsand
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
    
    for (int i = 0; i < text.length(); i++) {
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

bool TextEngine::canInsert(unsigned int line, unsigned int col) {
    return line < this->lines.size() &&
           col <= this->lines[line].text.length();
}

bool TextEngine::newLineAfter(unsigned int line) {
    if (line > this->lines.size()) {
        line = this->lines.size();
    }
    this->lines.insert(this->lines.begin() + line + 1, Line::create());
    return true;
}

bool TextEngine::insert(unsigned int line, unsigned int col, char to_insert) {
    if (!this->canInsert(line, col)) {
        std::cerr << "cannot insert at " << line << ":" << col << "!" << std::endl;
        return false;
    }
    if (to_insert == '\n') { // handle newline insertion
        std::string& line_ref = this->lines[line].text;
        if (col == line_ref.length()) {
            // if we're at the end of a line, all we need to do is insert an empty line after this.
            return this->newLineAfter(line);
        } else {
            std::string after = line_ref.substr(col);
            // erase everything after the cursor...
            line_ref.erase(col, std::string::npos);
            // and put in on the next line.
            this->lines.insert(this->lines.begin() + line + 1, Line::from(after));
        }
        return true;
    } else if (iscntrl(to_insert)) { // don't allow any other control characters
        std::cerr << "cannot insert invalid character" << std::hex << to_insert << std::endl;
        return false;
    } else { // if the character is anything else, insert it normally.
        std::string& r = this->lines[line].text;
        r.insert(r.begin() + col, to_insert);
        return true;  
    }
}



bool TextEngine::insertStr(unsigned int line, unsigned int col, std::string_view str) {
    if (!this->canInsert(line, col)) {
        std::cerr << "cannot insert at " << line << ":" << col << "!" << std::endl;
        return false;
    }
    auto new_lines = splitIntoLines(str);
    if (!new_lines) return false;

    // This needs to change
    //    line:{start}{end}
    // and turn it into
    //    line: {start}{input[0]}
    //  line+n: {input[n]]}
    //    last: {input[last]}{end}
    // so its kinda complicated and confusing :(

    std::string& current_line = this->lines[line].text;

    // optimization: if it is only one line, just insert
    if (new_lines->size() == 1) {
        current_line.insert(col, (*new_lines)[0].text);
        return true;
    }

    auto new_start = current_line.substr(0, col);
    new_start.append((*new_lines)[0].text);

    auto rest_of_line = current_line.substr(col, current_line.length());
    std::string& last = new_lines->rbegin()[0].text;
    // is this better than .append? no idea lmao
    last.insert(last.end(), rest_of_line.begin(), rest_of_line.end());

    // modify list
    (this->lines.begin() + line)[0].text = new_start;

    current_line = new_start;
    this->lines.insert(this->lines.begin() + line + 1, new_lines->begin() + 1, new_lines->end());

    return true;
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

bool TextEngine::deleteChar(unsigned int line, unsigned int col) {
    if (!this->canInsert(line, col)) return false;
    this->lines[line].text.erase(col, 1);
    return true;
}

bool TextEngine::deleteRange(unsigned int line, unsigned int start, unsigned int end) {
    if (
        !this->canInsert(line, start) ||
        !this->canInsert(line,end) ||
        end <= start
    ) {
        return false;
    }

    std::string& line_ref = this->lines[line].text;
    line_ref.erase(line_ref.begin() + start, line_ref.begin() + end + 1);
    return true;
}

bool TextEngine::deleteRange(unsigned int line_from, unsigned int col_from, unsigned int line_to, unsigned int col_to) {
    if (line_from == line_to) {
        return this->deleteRange(line_from, col_from, col_to);
    }
    
    std::string& start_line = this->lines[line_from].text;
    std::string& end_line = this->lines[line_to].text;

    auto new_start = start_line.substr(0, col_from);
    auto new_end = end_line.substr(col_to);

    this->lines[line_from].text = new_start + new_end;
    this->lines.erase(this->lines.begin() + line_from + 1, this->lines.begin() + line_to + 1);

    return false;
}

} // namespace soul
