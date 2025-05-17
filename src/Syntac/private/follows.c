#include <string.h>
#include <stdlib.h>
#include "syntac_internal.h"
#include "../../helpers/log/warn.h"

//Each rule has a first set of tokens reachable from that rule, which may not be "complete" for all that share the same rule_name
//	this helper function collects them all into a single set for all rules that share a name
char **get_firsts_of(struct stc_book *book, char *rule_name) {
	if (book == NULL) HLT_AERR("Book supplied null?");
	if (rule_name == NULL || rule_name[0] == '\0') HLT_AERR("Rule name supplied null/empty?");

	char **first = NULL;
	for (int k = 0; k < book->rule_count; k++) {
		if (strcmp(book->rules[k].name, rule_name) == 0) {
			for (int i = 0; book->rules[k].first_set[i] != NULL; i++) 
				SetAdd(&first, book->rules[k].first_set[i]);
		}
	}

	return first;
}

void follow_of_rule(struct stc_book *book, int rule_index) {
	if (book == NULL) HLT_AERR("Book supplied null?");
	if (rule_index < 0 || rule_index >= book->rule_count) HLT_AERR("Rule index out-of-bounds?");

	struct stc_rule *rule = &book->rules[rule_index];
	if (rule->follow_set != NULL) HLT_AERR("Rule %s already had a follow set generated?", rule->name);

	//Search for previously generated set
	for (int i = 0; i < book->rule_count; i++) {
		if (i == rule_index) continue;
		if (strcmp(rule->name, book->rules[i].name) == 0) {
			if (book->rules[i].follow_set != NULL) {
				rule->follow_set = SetUnion(NULL, book->rules[i].follow_set);
				return;
			}
		}
	}

	//scan each rule
	char **follow = NULL;
	for (int i = 0; i < book->rule_count; i++) {
		//NOTE: Can't skip the rule_index because what if "this" has itself in right side?

		//scan every element in right side
		for (int j = 0; book->rules[i].elements[j] != NULL; j++) {
			//We are looking for: "rule->name" "token/rule"

			char *elm = book->rules[i].elements[j];
			if (elm != NULL && strcmp(rule->name, elm) != 0) continue; //Element not this rule

			char *next = book->rules[i].elements[j+1];
			if (next != NULL && strcmp(rule->name, next) == 0) continue; //Follow up elm is this rule, skip forward

			if (next == NULL) { //Rule 3: Follow(book->rules[i]) into Follow(rule)
				//attempting to calculate this rule's own follow, currently doing so, skip
				if (strcmp(book->rules[i].name, rule->name) == 0) continue; 
				if (book->rules[i].follow_set == NULL) follow_of_rule(book, i);

				char **lcl = SetUnion(follow, book->rules[i].follow_set);
				if (follow != NULL) SetFree(follow);

				follow = lcl;
			} else { 
				//Rule 2: Terminals are follows ofc
				if (is_terminal(book, next)) SetAdd(&follow, next);
				else {
					bool move_on = false;
					int k = j+1;
					while (!move_on) {
						move_on = true; //Rule 2: First doesn't contain e, keep scanning normally
						char **first = get_firsts_of(book, book->rules[i].elements[k]);
						if (first == NULL) HLT_ERR("No first set for %s rule?", next);

						//Rule 3: First contains e, possible to have firsts of next elm
						if (SetContains(first, EPSILON)) {
							printf("%s: epsilon in %s's firsts: ", rule->name, book->rules[i].name);
							SetPrint(first);
						}
						if (SetRemove(&first, EPSILON)) move_on = false; 
	
						char **lcl = SetUnion(follow, first);
						if (follow != NULL) SetFree(follow);
						follow = lcl;
						
						SetFree(first); first = NULL; //unioned, can now ignore
						k++; //for r3
						
						if (!move_on && book->rules[i].elements[k] == NULL) { //means that we do Rule 3
							printf("%s: can't move forward on %s, doing rule 3\n\n", rule->name, book->rules[i].name);
							if (strcmp(book->rules[i].name, rule->name) == 0) continue; //skip
							if (book->rules[i].follow_set == NULL) follow_of_rule(book, i);
							
							lcl = SetUnion(follow, book->rules[i].follow_set);
							if (follow != NULL) SetFree(follow);
							follow = lcl;

							break; //cannot move on (end of elms), so must move on
						}
					}
				}
			}
		}
	}

	if (follow == NULL) rule->follow_set = SetCreate(1, ENDMRKR); //Rule 1: Starting symbol(s) has end-marker only
	else rule->follow_set = follow;
}

void follow_of_book(struct stc_book *book) {
	if (book == NULL) HLT_AERR("Book supplied null?");

	for (int i = 0; i < book->rule_count; i++) {
		if (book->rules[i].first_set == NULL) HLT_AERR("First set not generated before follow set?");
		if (book->rules[i].follow_set == NULL) follow_of_rule(book, i);
	}
}
