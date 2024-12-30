#include <string.h>
#include <stdio.h>
#include "syntac_internal.h"

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
		char **new_first = NULL;
		for (int j = 0; j < book->rule_count; j++) {
			if (strcmp(book->rules[j].name, elm) != 0) continue;
			if (i == j) continue; //loops into same rule are ignored (a -> a b a c, only b & c considered)

			if (book->rules[j].first_set == NULL) firsts_of_rule(book, &book->rules[j]);
			if (book->rules[j].first_set == NULL) continue; //failed to actually generate a first set, so we can't acquire it ourselves

			// NOTE: if two rules: a -> a b & a -> a d, still counted.... sooooo there needs to be a 'a -> ' rule that wipes out 'a' tokens/non-terminals
			if (book->rules[j].first_set == (void*)0x1) { 
				char errmsg[100];
				snprintf(errmsg, 100, "Grammar loop detected from rule '%s' into rule '%s'", rule->name, book->rules[j].name);
				HLT_WRN(errmsg, HLT_STDWRN);
				continue; //cannot use it, currently trying to finish it
			}
			
			//At this point in the code:
			//  It's the rule we are looking for,
			//	not our own rule, 
			//	and has a properly generated first set we can acquire

			//Acquire the first of this rule, add it to this rule
			char **lcl_first = SetUnion(new_first, book->rules[j].first_set);
			SetFree(new_first);
			new_first = lcl_first;
		}

		if (new_first != NULL) {
			rule->first_set = new_first;
			break;
		}
	}

	//check here
	if (rule->first_set == (void*)0x1) {
		char errmsg[100];
		snprintf(errmsg, 100, "Failed to generate a first set for rule '%s'", rule->name);
		HLT_WRN(errmsg, HLT_MJRWRN);

		rule->first_set = NULL;
	}
}

void firsts_of_book(struct stc_book *book) { //local use, all asserts/ifs should error
	if (book == NULL) HLT_ERR("Book supplied is null?");

	for (int i = 0; i < book->rule_count; i++) {
		if (book->rules[i] == NULL) HLT_AERR("How is there no rule at this spot?");
		if (book->rules[i].first_set == NULL) firsts_of_rule(book, &book->rules[i]);
	}
}
