#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		exit(1);
	}

	typedef struct {
		size_t line;
		size_t column;
	} lc_pair;

	lc_pair *open_loop_array;
	size_t open_loops = 0;

	size_t line = 0;
	size_t column = 0;

	static const char support_code_begin[] = "#include <stdio.h>\n\nchar cells[10000];\nchar *p = cells;\n\nint main() {\n";
	static const size_t support_code_begin_size = 70;

	static const char support_code_end[] = "\n}\n";
	static const size_t support_code_end_size = 3;

	static const char cell_plus[] = "(*p)++;\n";
	static const size_t cell_plus_size = 8;

	static const char cell_minus[] = "(*p)--;\n";
	static const size_t cell_minus_size = 8;

	static const char p_plus[] = "p++;\n";
	static const size_t p_plus_size = 5;

	static const char p_minus[] = "p--;\n";
	static const size_t p_minus_size = 5;

	static const char loop_begin[] = "while (*p) {\n";
	static const size_t loop_begin_size = 13;

	static const char loop_end[] = "}\n";
	static const size_t loop_end_size = 2;

	static const char in[] = "*p = (char) getchar();";
	static const size_t in_size = 22;

	static const char out[] = "putchar(*p);";
	static const size_t out_size = 12;


	FILE *in_file = fopen(argv[1], "r");
	
	if (ferror(in_file)) {
		perror("Error with the input file:");
		exit(1);
	}

	FILE *out_file = fopen(argv[2], "w");

	if (ferror(out_file)) {
		perror("Error with the output file:");
		exit(1);
	}

	fwrite(support_code_begin, sizeof(char), support_code_begin_size, out_file);
	printf(support_code_begin);

	while (!feof(in_file)) {
		char c = getc(in_file);
		column++;

		switch (c) {
			case '+':
				fwrite(cell_plus, sizeof(char), cell_plus_size, out_file);
				printf(cell_plus);
				break;
			case '-':
				fwrite(cell_minus, sizeof(char), cell_minus_size, out_file);
				printf(cell_minus);
				break;
			case '>':
				fwrite(p_plus, sizeof(char), p_plus_size, out_file);
				printf(p_plus);
				break;
			case '<':
				fwrite(p_minus, sizeof(char), p_minus_size, out_file);
				printf(p_minus);
				break;
			case '[':
				fwrite(loop_begin, sizeof(char), loop_begin_size, out_file);
				printf(loop_begin);

				open_loops++;
				
				if (open_loops == 1) {
					open_loop_array = (lc_pair*) malloc(sizeof(lc_pair));
				} else {
					open_loop_array = (lc_pair*) realloc(open_loop_array, open_loops * sizeof(lc_pair));
				}

				open_loop_array[open_loops - 1].line = line;
				open_loop_array[open_loops - 1].column = column;
				
				break;
			case ']':
				if (open_loops == 0) {
					printf("[%d:%d] Error: A loop was closed but never opened\n", line, column);

					fclose(in_file);
					fclose(out_file);

					remove(argv[2]);

					exit(1);
				} else {
					open_loops--;

					if (open_loops == 0) {
						free(open_loop_array);
					} else {
						open_loop_array = (lc_pair*) realloc(open_loop_array, open_loops * sizeof(lc_pair));
					}
				}

				fwrite(loop_end, sizeof(char), loop_end_size, out_file);
				printf(loop_end);

				break;
			case ',':
				fwrite(in, sizeof(char), in_size, out_file);
				printf(in);
				break;
			case '.':
				fwrite(out, sizeof(char), out_size, out_file);
				printf(out);
				break;
			case '\n':
				line++;
				column = 0;
				break;
		}
	}

	if (open_loops > 0) {
		if (open_loops == 1) {
			printf("[%d:%d] Error: A loop was opened but never closed\n", open_loop_array[0].line,
				open_loop_array[0].column);
		} else {
			for (size_t i = 0; i < open_loops; i++) {
				printf("[%d:%d] ", open_loop_array[i].line, open_loop_array[i].column);
			}

			printf("Error: Multiple loops were opened but never closed\n");
		}

		free(open_loop_array);
		fclose(in_file);
		fclose(out_file);
		remove(argv[2]);
		exit(1);
	}

	fwrite(support_code_end, sizeof(char), support_code_end_size, out_file);
	printf(support_code_end);

	fclose(in_file);
	fclose(out_file);
}