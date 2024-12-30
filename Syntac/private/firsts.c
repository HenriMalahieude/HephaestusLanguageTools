#include <string.h>
#include <stdio.h>
#include "syntac_internal.h"

//receives null-terminated sequence of c-strings
char ** firsts_from_sequence(struct stc_book *book, char **sqc) {
	if (book == NULL) HLT_ERR("Book supplied is null?");
	if (sqc == NULL) HLT_ERR("Sequence supplied is null?");
	
	//TODO
}

void firsts_of_rule(struct stc_book *book, struct stc_rule *rule) {
	if (book == NULL) HLT_ERR("Book supplied is null?");
	if (rule == NULL) HLT_ERR("Rule supplied is null?");
	if (rule->first_set != NULL) HLT_ERR("Redoing first set construction?");
	
	rule->first_set = (void*)0x1; //set in construction

	//epsilon/null/empty rules
	if (rule->elements == NULL || rule->elements[i] == NULL) {
		SetAdd(&rule->first_set, EPSILON);
		return;
	}
	
	for (int i = 0; rule->elements[i] != NULL; i++) {
		char *elm = rule->elements[i];

		if (is_terminal(elm)) {
			SetAdd(&rule->first_set, elm);
			break;
		}
		
		//Check each other (matching name/left) rule and get their first sets
		for (int j = 0; j < book->rule_count; j++) {
			if (strcmp(book->rules[j].name, elm) != 0) continue;
			if (i == j) continue; //loops into same rule are ignored (a -> a b a c, only b & c considered)

			if (book->rules[j].first_set == NULL) firsts_of_rule(book, &book->rules[j]);

			// NOTE: if two rules: a -> a b & a -> a d, still counted.... sooooo there needs to be a 'a -> ' rule that wipes it out
			if (book->rules[j].first_set == (void*)0x1) { 
				char errmsg[100];
				snprintf(errmsg, 100, "Grammar loop detected from rule '%s' into rule '%s'", rule->name, book->rules[j].name);
				HLT_WRN(errmsg, HLT_STDWRN);
				continue; //cannot use it, currently trying to finish it
			}

			//It's the rule we are looking for, not our own rule, and has a properly generated first set we can acquire
			
		}

		break;
	}

	//check here
	if (rule->first_set == (void*)0x1) HLT_ERR("Failed to generate a First Set for this Grammar Book.");
}

void firsts_of_book(struct stc_book *book) {
	if (book == NULL) HLT_ERR("Book supplied is null?");

	for (int i = 0; i < book->rule_count; i++) {
		if (book->rules[i] == NULL) HLT_AERR("How is there no rule at this spot?");
		if (book->rules[i].first_set == NULL) firsts_of_rule(book, &book->rules[i]);
	}
}
