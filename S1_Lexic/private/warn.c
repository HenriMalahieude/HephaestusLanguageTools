#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexic.h"

void Lexic_Error(char *msg) {
	printf("Lexic Error @ L%d, C%d: %s\n", regex_line_no, regex_colu_no, msg);
	exit(1);
}

enum lex_warn_type warn_level = LWT_STDWRN;
void Lexic_Warn(char *msg, enum lex_warn_type type) {
	if (type == LWT_SILENT) Lexic_Error("Attempted to make a silent warning?");

	if ((int)type > (int)warn_level) return;
	if (type == LWT_DEBUG) printf("%s\n", msg);
	else printf("Lexic Warning @ L%d, C%d: %s\n", regex_line_no, regex_colu_no, msg);
}
