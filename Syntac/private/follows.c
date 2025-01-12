#include <string.h>
#include <stdlib.h>
#include "syntac_internal.h"
#include "../../helpers/log/warn.h"

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


	//Generate the follow set
	char **follow = NULL;
	for (int i = 0; i < book->rule_count; i++) {
		for (int j = 0; book->rules[i].elements[j] != NULL; j++) {
			if (strcmp(rule->name, book->rules[i].elements[j]) == 0) {
				char *next = book->rules[i].elements[j+1];
				if (next == NULL) { //Rule 3: Follow(rule[i]) in Follow(rule[rule_index])
					if (book->rules[i].follow_set == NULL) follow_of_rule(book, i);
					char **lcl = SetUnion(follow, book->rules[i].follow_set);
					if (follow != NULL) SetFree(follow);
					follow = lcl;
				} else { //Rule 2/4: First(elm) - epsilon in Follow(rule[rule_index])
					if (is_terminal(book, next)) SetAdd(&follow, next);
					else {
						char **first = get_firsts_of(book, next);
						if (first == NULL) HLT_AERR("No first set for %s rule?", next);
						
						//TODO
						if (!SetContains(first, EPSILON)) { //Rule 2: First(elm) in Follow
						} else { //Rule 3: First
						}
					}
				}
			}
		}
	}

	if (follow == NULL) 	rule->follow_set = SetCreate(1, ENDMRKR); //Rule 1: Starting symbol has end-marker only
	else 			rule->follow_set = follow;
}

void follow_of_book(struct stc_book *book) {
	if (book == NULL) HLT_AERR("Book supplied null?");

	for (int i = 0; i < book->rule_count; i++) {
		if (book->rules[i].first_set == NULL) HLT_AERR("First set not generated before follow set?");
		if (book->rules[i].follow_set == NULL) follow_of_rule(book, i);
	}
}
