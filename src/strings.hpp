#if !defined(STRINGS_H)
#define STRINGS_H

#include <string>

static const std::string support_code_begin = "#include <stdio.h>\nchar cells[10000];char *p=cells;int main(){";
static const std::string support_code_begin_pretty = "#include <stdio.h>\n\nchar cells[10000];\nchar *p = cells;\n\nint main() {";
static const std::string support_code_end = "}";

static const std::string cell_increment = "(*p)+=";
static const std::string cell_decrement = "(*p)-=";

static const std::string pointer_increment = "p+=";
static const std::string pointer_decrement = "p-=";

static const std::string loop_begin_pretty = "while (*p) {";
static const std::string loop_begin = "while(*p){";
static const std::string loop_end = "}";

static const std::string char_in = "*p=(char)getchar();";
static const std::string char_out = "putchar(*p);";

#endif // STRINGS_H
