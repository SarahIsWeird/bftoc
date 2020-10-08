#include <fstream>
#include <iostream>
#include <tclap/CmdLine.h>

#include "preprocessor.h"
#include "processor.h"

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

    std::string preprocessed_contents = preprocessor::preprocess(in_str);

    ///////////////////////////////////////
    ////          Processing           ////
    ///////////////////////////////////////

    std::string output = processor::process(preprocessed_contents, pretty_print);

    out_stream << output;

    out_stream.close();

    return 0;
}