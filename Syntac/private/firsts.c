#include <string.h>
#include <stdio.h>
#include "syntac_internal.h"

int firsts_recursion_limit = 0;

void firsts_of_rule(struct stc_book *book, struct stc_rule *rule) {
	if (book == NULL) HLT_ERR("Book supplied is null?");
	if (rule == NULL) HLT_ERR("Rule supplied is null?");

	if (firsts_recursion_limit >= FIRSTS_RCR_LIM) firsts_recursion_limit++;
	else HLT_ERR("Recursion limit hit on recursive function!\nTODO: Make this cmd line arg!");

	char *output[100];
	snprintf(output, 100, "Calculating the first set of '%s'", rule->name);
	HLT_WRN(output, HLT_VERBSE);
	
	//epsilon/null/empty rules
	if (rule->elements == NULL || rule->elements[0] == NULL) {
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

			firsts_of_rule(book, &book->rules[j]);
			if (book->rules[j].first_set == NULL) continue; //failed to actually generate a first set, so we can't acquire it ourselves
			
			//At this point in the code:
			//  It's the rule we are looking for,
			//	not our own rule, 
			//	and has a properly generated first set we can acquire

			//Acquire the first of this rule, add it to this rule
			char **lcl_first = SetUnion(new_first, book->rules[j].first_set);
			if (new_first != NULL) SetFree(new_first);
			new_first = lcl_first;
		}
	
		if (new_first != NULL) {

			bool can_empty = false;
			for (int i = 0; new_first[i] != NULL; i++) {
				if (strcmp(new_first[i], EPSILON) == 0) {
					can_empty = true; //don't add epsilon to the first
					if (book->elements[i+1] == NULL) SetAdd(&rule->first_set, EPSILON); //add it if no finality
				} else SetAdd(&rule->first_set, new_first[i]);
			}

			SetFree(new_first); new_first = NULL;
		}

		break;
	}

	//NOTE: Could be NULL because it's waiting on other rules?
}

void firsts_of_book(struct stc_book *book) { //local use, all asserts/ifs should error
	if (book == NULL) HLT_ERR("Book supplied is null?");
	
	int lim = 0;
	bool changed = true;
	while (changed) {
		if (lim < FIRSTS_CHG_LIM) lim++;
		else HLT_ERR("Recursion limit hit on changed status of first sets within grammar.\nTODO: Make this a cmd line arg");

		changed = false;
		for (int i = 0; i < book->rule_count; i++) { 
			if (book->rules[i] == NULL) HLT_AERR("How is there no rule at this spot?");
			
			int set_sz = SetCount(book->rules[i].first_set);
			firsts_of_rule(book, &book->rules[i]);
			if (set_sz != SetCount(book->rules[i].first_set)) changed = true;
			firsts_recursion_limit = 0;
		}
	}
	
	//Then we check that we got everything to have a firsts set
	bool failed = false;
	for (int i = 0; i < book->rule_count; i++) {
		if (book->rules[i].first_set == NULL) {
			char errmsg[100];
			snprintf(errmsg, 100, "Couldn't generate the first set of rule '%s'?", book->rules[i].name);
			HLT_WRN(errmsg, HLT_MJRWRN);
			failed = true;
		}
	}
	
	//TODO: Insert command line argument placement here
	if (failed) HLT_ERR("Some first sets were not generated, not told to ignore.");
}
