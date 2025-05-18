#include "syntac_internal.h"
#include "ll1.h"

void CalculateLL1Table(SyntacBook *book) {
	if (book->type != STC_LL1) HLT_AERR("Attempted to calculate LL1 table for non-LL1 grammar book?");
	HLT_WRN(HLT_VERBSE, "Generating LL1 Parsing Table for the book!");

	//Validate
	for (int i = 0; i < book->rule_count; i++) {
		//Rule 1: No Left Recursion (A -> A : B)
		if (strcmp(book->rules[i].name, book->rules[i].elements[0]) == 0) 
			HLT_ERR("LL1 Grammar has left recursion on %s (#%d). Cannot calculate table.", book->rules[i].name, i);

		for (int j = i+1; j < book->rule_count; j++) {
			//Rule 2: Unambiguous (first sets are unique)
			if (SetOverlaps(book->rules[i].first_set, book->rules[j].first_set))
				HLT_ERR("LL1 Grammar is ambiguous. Rule %d's (%s) firsts intersect with rule %d's (%s) firsts. Cannot calculate table.", 
						i, book->rules[i].name, j, book->rules[j].name);

			//Rule 3: Disjoint first and follow sets
			if (SetOverlaps(book->rules[i].first_set, book->rules[j].follow_set)) 
				HLT_ERR("LL1 Grammar has intersection between %s's (#%d) firsts, and %s's (#%d) follows. Cannot calculate table.",
						book->rules[i].name, i, book->rules[j].name, j);

			if (SetOverlaps(book->rules[i].follow_set, book->rules[j].first_set))
				HLT_ERR("LL1 Grammar has intersection between %s's (#%d) follows, and %s's (#%d) firsts. Cannot calculate table.", 
						book->rules[i].name, i, book->rules[j].name, j);
		}
	}

	//Generate entries
	book->ll1_table = (struct stc_ll1_table_entry *)malloc(sizeof(struct stc_ll1_table_entry));
	book->ll1_table.entries = NULL;
	book->ll1_table.entry_cnt = 0;
	for (int i = 0; i < book->rule_count; i++) {
	
	}

	book->ll1_table = tbl;
}

struct stc_tree_node * TopParseStream(char **stream, SyntacBook *book) {
	if (book->ll1_table == NULL) CalculateLL1Table(book);
}

struct stc_tree_node * TopParseTokens(void *stream, SyntacBook *book) {
	if (book->ll1_table == NULL) CalculateLL1Table(book);
}
