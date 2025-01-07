#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "syntac_internal.h"
#include "../../helpers/log/warn.h"

int *tracker = NULL;
int firsts_loop_limit = 0;

void firsts_of_rule(struct stc_book *book, int rule_index) {
	if (book == NULL) HLT_ERR("Book supplied is null?");
	if (rule_index < 0 || rule_index >= book->rule_count) HLT_ERR("Rule supplied is null?");

	struct stc_rule *rule = &book->rules[rule_index];

	if (tracker == NULL) HLT_AERR("Internal error, tracking state for 'dynamic' programming failed?");
	if (tracker[rule_index] >= firsts_loop_limit) return; //already calculated for this loop cycle
	tracker[rule_index] = firsts_loop_limit; //about to calculate it for this loop cycle

	HLT_WRN(HLT_DEBUG, "Calculating the first set of %s", rule->name);
	
	//epsilon/null/empty rules
	if (rule->elements == NULL || rule->elements[0] == NULL) {
		SetAdd(&rule->first_set, EPSILON);
		HLT_WRN(HLT_DEBUG, "Firsts of %s exited with epsilon first", rule->name);
		return;
	}
	
	for (int i = 0; rule->elements[i] != NULL; i++) {
		char *elm = rule->elements[i];

		if (is_terminal(book, elm)) {
			SetAdd(&rule->first_set, elm);
			break;
		}
		
		//Check each other (matching name/left) rule and get their first sets
		char **new_first = NULL;
		for (int j = 0; j < book->rule_count; j++) {
			if (strcmp(book->rules[j].name, elm) != 0) continue;
			if (i == j) continue; //loops into same rule are ignored (a -> a b a c, only b & c considered)
			
			firsts_of_rule(book, j);
			if (book->rules[j].first_set == NULL) continue; //failed to actually generate a first set, so we can't acquire it ourselves
			
			//At this point in the code:
			//  It's the rule we are looking for,
			//	not ourselves (rule), 
			//	and has a properly generated first set we can acquire

			//Acquire the first of this rule, add it to ourselves
			char **lcl_first = SetUnion(new_first, book->rules[j].first_set);
			if (new_first != NULL) SetFree(new_first);
			new_first = lcl_first;
		}
	
		if (new_first != NULL) {
			printf("\t\tAdding to firsts! Disallow epsilon add except at end!\n");
			for (int i = 0; new_first[i] != NULL; i++) {
				if (strcmp(new_first[i], EPSILON) == 0) {
					printf("\t\t\tThere was an epsilon!\n");
					if (rule->elements[i+1] == NULL) {
						printf("\t\t\t\tAdded it anyways!\n");
						SetAdd(&rule->first_set, EPSILON); //add it if no finality
					}
				} else SetAdd(&rule->first_set, new_first[i]);
			}

			SetFree(new_first); new_first = NULL;
		}
	}

	HLT_WRN(HLT_DEBUG, "Firsts of %s exited with %d firsts", rule->name, SetCount(rule->first_set));
	//NOTE: Could be NULL because it's waiting on other rules
}

void firsts_of_book(struct stc_book *book) { //local use, all asserts/ifs should error
	if (book == NULL) HLT_ERR("Book supplied is null?");

	HLT_WRN(HLT_DEBUG, "Top of Recursive call stack for Firsts!!!!!!");

	if (tracker != NULL) {
		free(tracker); tracker = NULL;
	}

	tracker = calloc(book->rule_count, sizeof(int)); //calloc sets all to zero
	
	bool changed = true;
	while (changed) {
		if (firsts_loop_limit < FIRSTS_CHG_LIM) firsts_loop_limit++;
		else HLT_ERR("Recursion limit hit on changed status of first sets within grammar.\nTODO: Make this a cmd line arg");

		changed = false;
		for (int i = 0; i < book->rule_count; i++) { 
			int set_sz = SetCount(book->rules[i].first_set);
			firsts_of_rule(book, i);
		
			if (set_sz != SetCount(book->rules[i].first_set)) changed = true;
		}
	}
	
	//Then we check that we got everything to have a firsts set
	bool failed = false;
	for (int i = 0; i < book->rule_count; i++) {
		if (book->rules[i].first_set == NULL) {
			HLT_WRN(HLT_MJRWRN, "Couldn't generate the first set of rule '%s'?", book->rules[i].name);
			failed = true;
		}
	}
	
	//TODO: Some wait to control this failed signal
	if (failed) HLT_ERR("Some first sets were not generated, not told to ignore.");

	firsts_loop_limit = 0;
	free(tracker); tracker = NULL;
}
