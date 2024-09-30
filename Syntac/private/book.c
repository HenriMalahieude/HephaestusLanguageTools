#include <stdlib.h>
#include <string.h>

#include "syntac_private.h"
#include "../../helpers/string/handy.h"
#include "../../helpers/log/warn.h"

//Allocate an Empty Rule Book
SyntacBook * SyntacBookAllocate() {
	SyntacBook *book = malloc(sizeof(struct stc_book));
	
	book->rules = NULL;
	book->rule_count = 0;

	return book;
}

//Free a rule book (assumes everything within the rulebook and rules is dynamically allocated)
void SyntacBookFree(SyntacBook *book) {
	if (book == NULL) {
		HLTWarn("SyntacBookFree. Supplied book pointer was null?", 0, 0, HLT_STDWRN);
		return;
	}

	for (int i = 0; i< book->rule_count; i++){
		struct stc_rule *rule = &book->rules[i];
		if (rule == NULL) {
			HLTWarn("SyntacBookFree. Rule pointer was null?", 0, 0, HLT_MJRWRN);
			continue;
		}

		free(rule->name); rule->name = NULL;
		for (int j = 0; rule->elements[j] != NULL; j++) {
			free(rule->elements[j]); //free all elements inside
		}
		free(rule->elements); rule->elements = NULL; //free container
	}

	if (book->rules != NULL) free(book->rules); 
	book->rules = NULL; book->rule_count = 0;
	free(book);
}	

void SyntacBookRuleAdd(SyntacBook *book, char *left, char *right) {
	if (book == NULL) {
		HLTWarn("SyntacBookRuleAdd. Supplied book pointer was null?", 0, 0 HLT_STDWRN);
		return;
	}

	if (left == NULL || left[0] == '\0') {
		HLTWarn("SyntacBookRuleAdd. Supplied left of rule was null/empty?", 0, 0, HLT_STDWRN);
		return;
	}

	if (right == NULL || left[0] == '\0') {
		HLTWarn("SyntacBookRuleAdd. Supplied right of rule was null/empty?", 0, 0, HLT_STDWRN);
		return;
	}

	struct stc_rule *rule = malloc(sizeof(struct stc_rule));
	rule->name = calloc(strlen(left)+1, sizeof(char)); strcpy(left_copy, left);

	int elm_cnt = 0; bool lstwasSpace = true;
	for (int i = 0; i < strlen(right); i++) {
		if (isspace(right[i])) {
			if (!lstspace) elm_cnt++;
			lstwasSpace = true;
		} else {
			lstwasSpace = false;
		}
	}
		
	rule->elements = malloc(sizeof(char *) * (elm_cnt+1));
	rule->elements[elm_cnt] = NULL;

	//TODO: Actually parse out the elements (dynamically allocate them too)
}

SyntacBook * SyntacBookFromFile(char *file_name, SyntacTreeType type){
	if (file_name == NULL || file_name[0] == '\0') {
		HLTWarn("SyntacBookFromFile. Supplied file_name was null/empty?");
		return NULL;
	}

	char *cntnts = ftostr(file_name);
		
	SyntacBook *book = SyntacBookFromString(cntnts);
	
	free(cntnts);
	return book;
}

SyntacBook * SyntacBookFromString(char *stream, SyntacTreeType type) {
	//TODO
}

