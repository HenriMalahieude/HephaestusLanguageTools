#include <string.h>
#include <stdlib.h>

#include "warn.h"

enum hlt_warn_type warn_level = HLT_STDWRN;

void append_line_no(int line, int col);

void HLTError(char *msg, int line, int col) {
	printf("Error");
	append_line_no(line, col);
	printf(": %s\n", msg);
	exit(1);
}

void HLTWarn(char *msg, int line, int col, enum hlt_warn_type type) {
	if (type == HLT_SILENT) HLT_AERR("Attempted to make a silent warning?");

	if ((int)type > (int)warn_level) return;
	if (type == HLT_DEBUG) printf("%s\n", msg);
	else {
		printf("Warning");
		append_line_no(line, col);
		printf(": %s\n", msg);
	}
}

void append_line_no(int line, int col) {
	if (line < 0 && col < 0) return;
	printf(" @");
	if (line >= 0) printf(" Line: %d", line);
	if (line >= 0 && col >= 0) printf(",");
	if (col >= 0)  printf(" Column:%d", col);
}
