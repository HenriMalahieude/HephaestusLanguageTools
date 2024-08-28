#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "warn.h"

void HLTError(char *msg, int line, int col) {
	printf("[!] Error @ L%d, C%d: %s\n", line, col, msg);
	exit(1);
}

enum hlt_warn_type warn_level = HLT_STDWRN;

void HLTWarn(char *msg, int line, int col, enum hlt_warn_type type) {
	if (type == HLT_SILENT) HLTError("Attempted to make a silent warning?", -1, -1);

	if ((int)type > (int)warn_level) return;
	if (type == HLT_DEBUG) printf("%s\n", msg);
	else printf("[?] Warning @ L%d, C%d: %s\n", line, col, msg);
}
