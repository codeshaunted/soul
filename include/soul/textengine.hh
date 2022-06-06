#ifndef SOUL_TEXTENGINE_HH
#define SOUL_TEXTENGINE_HH

#include <string>
#include <vector>
#include <span>
#include <optional>

#include "tl/expected.hpp"

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
    std::string to_string();
    std::optional<std::vector<std::string_view>> get_lines(uint from, uint to);
    uint num_lines();
    bool can_insert(uint line, uint col);
    // return true if succeeded, otherwise false.
    bool new_line_after(uint line);
    bool insert(uint line, uint col, char to_insert);
    bool insert_str(uint line, uint col, std::string_view str);
    bool delete_char(uint line, uint col);
    bool delete_range(uint line, uint start, uint end);
    bool delete_range(uint line_from, uint col_from, uint line_to, uint col_to);

private:
    TextEngine(std::optional<std::vector<Line>> initial);
};

void d_print_lines(std::vector<Line>& v);
void d_print_vec(std::vector<std::string>& v);

#endif // SOUL_TEXTENGINE_HH