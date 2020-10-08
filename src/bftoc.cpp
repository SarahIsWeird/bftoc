#include <fstream>
#include <iostream>
#include <unistd.h>
#include <tclap/CmdLine.h>

#include "strings.hpp"
#include "preprocessor.h"

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

std::string make_indents(int n) {
    std::string str;

    // n+1: Account for normal indentation.
    for (int i = 0; i < n + 1; ++i) {
        str += "  ";
    }

    return str;
}

int main(int argc, char **argv) {
    std::string in_str;
    std::string preprocessed_str;

    std::string in_path;
    std::string out_path;

    bool pretty_print;

    try {
        TCLAP::CmdLine cmd("(c) 2020 Sarah Klocke.", ' ', "2.1");

        TCLAP::UnlabeledValueArg<std::string> in_path_arg("input", "Path to the input file", true, "", "File...");
        TCLAP::ValueArg<std::string> out_path_arg("o", "output", "Path to the output file", false, "out.c", "File...");
        TCLAP::SwitchArg pretty_print_arg("b", "beautify", "Don't minimize the C code.", false);

        cmd.add(in_path_arg);
        cmd.add(out_path_arg);
        cmd.add(pretty_print_arg);

        cmd.parse(argc, argv);

        in_path = in_path_arg.getValue();
        out_path = out_path_arg.getValue();
        pretty_print = pretty_print_arg.getValue();
    } catch (TCLAP::ArgException &e) {
        std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
        exit(1);
    }

    std::ifstream in_stream(in_path);

    if (!in_stream.is_open()) {
        std::cerr << in_path << " (input file) couldn't be opened.\n";
        exit(1);
    }
    
    in_str.assign(std::istreambuf_iterator<char>(in_stream), std::istreambuf_iterator<char>());

    std::ofstream out_stream(out_path);

    if (!out_stream.is_open()) {
        std::cerr << out_path << " (output file) couldn't be opened.\n";
        exit(1);
    }

    ///////////////////////////////////////
    ////        Pre-Processing         ////
    ///////////////////////////////////////

    std::string preprocessed_contents = preprocess(in_str);

    ///////////////////////////////////////
    ////          Processing           ////
    ///////////////////////////////////////

    std::string count;

    int open_loop_count = 0;

    if (pretty_print)
        out_stream << support_code_begin_pretty;
    else
        out_stream << support_code_begin;

    for (char &c : preprocessed_contents) {

        if (c > '0' && c < '9') {
            count += c;
            continue;
        }

        if (pretty_print)
            out_stream << make_indents(open_loop_count - (c == ']' ? 1 : 0));

        if (c == '[') {
            open_loop_count++;

            if (pretty_print)
                out_stream << "\n" << make_indents(open_loop_count - 1);
        } else if (c == ']') {
            open_loop_count--;
        }

        out_stream << get_block_for_character(c);

        if (std::string("+-<>").find(c) != std::string::npos)
            out_stream << count << ';';

        if (pretty_print)
            out_stream << "\n";

            if (c == ']')
                out_stream << "\n";

        count = "";
    }

    out_stream << support_code_end;

    out_stream.close();

    return 0;
}