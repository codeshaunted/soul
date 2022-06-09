// codeshaunted - soul
// include/soul/text_engine.hh
// text engine class header file
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

#ifndef SOUL_TEXT_ENGINE_HH
#define SOUL_TEXT_ENGINE_HH

#include <string>
#include <vector>
#include <span>
#include <optional>
#include <utility>

#include "tl/expected.hpp"

#include "error.hh"

namespace soul {

/**
 * represents a (line, column) pair.
 */
typedef std::pair<unsigned int, unsigned int> CurPos;

class Line {
    public:
        std::string text;

        ~Line();
        static Line create();
        static Line from(std::string _text);
    private:
        Line();
        Line(std::string _text);
};

// typedef unsigned int idx;

class TextEngine {
    // TODO: make stuff private?
    public:
        // TODO: better datastructure for this?
        std::vector<Line> lines;

        ~TextEngine();

        static TextEngine create();
        static tl::expected<TextEngine, std::string> from(std::string_view text);

        /**
         * convert the buffer to one string, separating lines with '\n'
         */
        std::string toString();
        std::optional<std::vector<std::string_view>> getLines(unsigned int from, unsigned int to);
        tl::expected<Line*, Error> getLine(unsigned int num);
        unsigned int numLines();
        bool canInsert(CurPos pos);
        // These all return the new position of a cursor after doing the
        // specified manipulation, e.g. after an inserted character.
        std::optional<CurPos> newLineAfter(unsigned int line);
        std::optional<CurPos> insert(CurPos pos, char to_insert);
        std::optional<CurPos> insertStr(CurPos pos, std::string_view str);
        // delete char *before* pos
        std::optional<CurPos> deleteChar(CurPos pos);
        // delete everything between start and end (inclusive) on given line
        std::optional<CurPos> deleteRange(unsigned int line, unsigned int start, unsigned int end);
        std::optional<CurPos> deleteRange(CurPos from, CurPos to);

    private:
        TextEngine(std::optional<std::vector<Line>> initial);
};

void dPrintLines(std::vector<Line>& v);
void dPrintVec(std::vector<std::string>& v);

} // namespace soul

#endif // SOUL_TEXT_ENGINE_HH