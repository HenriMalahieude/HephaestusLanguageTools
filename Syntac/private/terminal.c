#include <string.h>
#include "syntac_internal.h"

bool is_terminal(struct stc_book *book, char *element) {
	for (int i = 0; i < book->rule_count; i++) {
		if (strcmp(book->rules[i].name, element) == 0) return false;
	}

	return true;
}
