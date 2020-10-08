#ifndef BFTOC_PROCESSOR_H
#define BFTOC_PROCESSOR_H

#include <string>
#include <sstream>

#include "strings.h"

namespace processor {
    std::string get_block_for_character(char c, bool pretty_print);

    std::string make_indents(int n);

    std::string process(std::string preprocessed_in, bool pretty_print);
}

#endif //BFTOC_PROCESSOR_H
