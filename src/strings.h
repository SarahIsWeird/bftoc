#ifndef STRINGS_H
#define STRINGS_H

#define support_code_begin "#include <stdio.h>\nchar cells[10000];char *p=cells;int main(){"
#define support_code_begin_pretty "#include <stdio.h>\n\nchar cells[10000];\nchar *p = cells;\n\nint main() {"
#define support_code_end "}"

#define cell_increment_pretty "(*p) += "
#define cell_increment "(*p)+="
#define cell_decrement_pretty "(*p) -= "
#define cell_decrement "(*p)-="

#define pointer_increment_pretty "p += "
#define pointer_increment "p+="
#define pointer_decrement_pretty "p -= "
#define pointer_decrement "p-="

#define loop_begin_pretty "while (*p) {"
#define loop_begin "while(*p){"
#define loop_end_pretty "}\n"
#define loop_end "}"

#define char_in_pretty "*p = (char) getchar();"
#define char_in "*p=(char)getchar();"
#define char_out "putchar(*p);"

#endif // STRINGS_H
