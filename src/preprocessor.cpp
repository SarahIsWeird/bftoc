#include <sstream>
#include <iostream>

#include "preprocessor.h"

std::string preprocessor::preprocess(const std::string &in) {
    std::stringstream preprocessing_ss;

    char last_char = 0;
    size_t last_char_c = 0;

    long open_loop_count = 0;

    for (char c : in) {
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

    return preprocessing_ss.str();
}