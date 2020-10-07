#include <fstream>
#include <iostream>
#include <sstream>

#include "strings.hpp"

std::string get_block_for_character(char c) {
    switch (c) {
        case '+':
            return cell_increment;
        case '-':
            return cell_decrement;
        case '>':
            return pointer_increment;
        case '<':
            return pointer_decrement;
        case '[':
            return loop_begin;
        case ']':
            return loop_end;
        case ',':
            return char_in;
        case '.':
            return char_out;
        default:
            return "";
    }
}

int main(int argc, char **argv) {
    std::string in_str;
    std::string preprocessed_str;

	if (argc != 3) {
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		exit(1);
	}

    std::ifstream in_stream(argv[1]);

    if (!in_stream.is_open()) {
        std::cerr << argv[1] << " (input file) couldn't be opened.\n";
        exit(1);
    }
    
    in_str.assign(std::istreambuf_iterator<char>(in_stream), std::istreambuf_iterator<char>());

    std::ofstream out_stream(argv[2]);

    if (!out_stream.is_open()) {
        std::cerr << argv[2]<< " (output file) couldn't be opened.\n";
        exit(1);
    }

    ///////////////////////////////////////
    ////        Pre-Processing         ////
    ///////////////////////////////////////

    std::stringstream preprocessing_ss;

    char last_char = 0;
    size_t last_char_c = 0;

    long open_loop_count = 0;

    for (char &c : in_str) {
        if (c == '[')
            open_loop_count++;
        else if (c == ']')
            open_loop_count--;

        if (c == last_char && std::string("+-<>").find(c) != std::string::npos) {
            last_char_c++;
            continue;
        }

        if (c != 0) {
            preprocessing_ss << std::to_string(last_char_c) << last_char;

            last_char = c;
            last_char_c = 1;
        }
    }

    if (open_loop_count > 0) {
        std::cerr << "You forgot to close " << std::to_string(open_loop_count) << " loop"
            << (open_loop_count > 1 ? "s" : "") << "!\n";
        exit(1);
    } else if (open_loop_count < 0) {
        std::cerr << "You closed " << std::to_string(-open_loop_count) << " loop"
                  << (-open_loop_count > 1 ? "s" : "") << " which you never opened!\n";
        exit(1);
    }

    ///////////////////////////////////////
    ////          Processing           ////
    ///////////////////////////////////////

    std::string count;

    out_stream << support_code_begin;

    for (char &c : preprocessing_ss.str()) {

        if (c > '0' && c < '9') {
            count += c;
            continue;
        }

        out_stream << get_block_for_character(c);

        if (std::string("+-<>").find(c) != std::string::npos)
            out_stream << count << ';';

        count = "";
    }

    out_stream << support_code_end;

    out_stream.close();

    return 0;
}