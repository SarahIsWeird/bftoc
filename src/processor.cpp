#include "processor.h"

std::string processor::get_block_for_character(char c, bool pretty_print) {
    switch (c) {
        case '+':
            return pretty_print ? cell_increment_pretty : cell_increment;
        case '-':
            return pretty_print ? cell_decrement_pretty : cell_decrement;
        case '>':
            return pretty_print ? pointer_increment_pretty : pointer_increment;
        case '<':
            return pretty_print ? pointer_decrement_pretty : pointer_decrement;
        case '[':
            return pretty_print ? loop_begin_pretty : loop_begin;
        case ']':
            return pretty_print ? loop_end_pretty : loop_end;
        case ',':
            return char_in;
        case '.':
            return char_out;
        default:
            return "";
    }
}

std::string processor::make_indents(int n) {
    std::string str;

    // n+1: Account for normal indentation.
    for (int i = 0; i < n + 1; ++i) {
        str += "  ";
    }

    return str;
}

std::string processor::process(std::string preprocessed_in, bool pretty_print) {
    std::stringstream out_stream;

    std::string count;

    char last_char = ' ';

    int open_loop_count = 0;

    if (pretty_print)
        out_stream << support_code_begin_pretty;
    else
        out_stream << support_code_begin;

    for (char &c : preprocessed_in) {

        if (c > '0' && c < '9') {
            count += c;
            continue;
        }

        if (last_char != '.' && c == '.' && pretty_print)
            out_stream << "\n";
        else if (last_char == '.' && c != '.' && pretty_print)
            out_stream << "\n";

        if (pretty_print)
            out_stream << make_indents(open_loop_count - (c == ']' ? 1 : 0));

        if (c == '[') {
            open_loop_count++;

            if (pretty_print)
                out_stream << "\n" << make_indents(open_loop_count - 1);
        } else if (c == ']') {
            open_loop_count--;
        }

        out_stream << get_block_for_character(c, pretty_print);

        if (std::string("+-<>").find(c) != std::string::npos)
            out_stream << count << ';';

        if (pretty_print)
            out_stream << "\n";

        last_char = c;

        count = "";
    }

    out_stream << support_code_end;

    return out_stream.str();
}