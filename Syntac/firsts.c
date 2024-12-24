#include "syntac_internal.h"

bool is_terminal(struct stc_book *book, char *element) {
	for (int i = 0; i < book->rule_count; i++) {
		if (strcmp(book->rules[0].name, element) == 0) return true;
	}

	return false;
}

//receives null-terminated sequence of c-strings
char ** firsts_from_sequence(struct stc_book *book, char **sqc) {
	if (book == NULL) HLT_ERR("Book supplied is null?");
	if (sqc == NULL) HLT_ERR("Sequence supplied is null?");
	
	//TODO
}

void firsts_of_rule(struct stc_book *book, struct stc_rule *rule) {
	if (book == NULL) HLT_ERR("Book supplied is null?");
	if (rule == NULL) HLT_ERR("Rule supplied is null?");

	//TODO
}

void firsts_of_book(struct stc_book *book) {
	for (int i = 0; i < book->rule_count; i++) {
		if (book->rules[i] == NULL) HLT_AERR("How is there no rule at this spot?");
		if (book->rules[i].first_set == NULL) firsts_of_rule(book, &book->rules[i]);
	}
}
