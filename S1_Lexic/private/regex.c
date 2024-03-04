#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../lexic.h"
#include "regex.h"
#include "match.h"

void Regex_Error(char *msg) {
	printf("Regex Error @ L%d, C%d:\n%s\n", regex_line_no, regex_colu_no, msg);
	exit(1);
}

void Regex_Warning(char *msg) {
	printf("Regex Warning @ L%d, C%d:\n%s\n", regex_line_no, regex_colu_no, msg);
}

struct regex* Regex_New_Direct(char *match) {
	struct regex *def = (struct regex*)malloc(sizeof(struct regex));
	def->attached_data = match;
	def->match_function = Regex_Match_Direct;

	return def;
}

struct regex* Regex_New_Or(struct regex *left, struct regex *right) {
	struct regex *def = (struct regex*)malloc(sizeof(struct regex));
 	
	void *att = malloc(2*sizeof(struct regex*));
	((struct regex**)att)[0] = left;
	((struct regex**)att)[1] = right;
	def->attached_data = att;
	def->match_function = Regex_Match_Or;

	return def;
}

struct regex* Regex_New_Brackets(char *internals) {
	if (strlen(internals) <= 0) Regex_Error("New Brackets. internals cannot be len = 0\n");
	struct regex *def = (struct regex*)malloc(sizeof(struct regex));
	
	struct regex **bracket_internals = NULL;
	int bracket_count = 0;

	for (size_t i = 0; i < strlen(internals); i++) {
		char cur = internals[i];
		char nex = internals[i+1];
		if (internals[i+1] == '-') { //This is a sequence, no way for SIGSEGV bc [i+1] = '\0' at end
			char *seq = (char*)malloc(4*sizeof(char));
			seq[0] = cur; seq[1] = nex; seq[2] = internals[i+2]; seq[3] = '\0';
			struct regex* seq_regex = Regex_New_Sequence(seq);
			
			bracket_count++;
			bracket_internals = (struct regex **)realloc(bracket_internals, bracket_count * sizeof(struct regex *));
			if (bracket_internals == NULL) Regex_Error("realloc error in brackets\n");
			bracket_internals[bracket_count-1] = seq_regex;

			i += 2; //move i to i+2, on last char of seq. then eol will move i to after seq
		} else if (cur != '\0') { //This is a "literal"
			if (cur == '[' || cur == ']') Regex_Error("Bracket Rule within Bracket Rule not supported\n");

			struct regex *lit_regex; //ahaha, iz littt
			char *lit = (char*)malloc(3*sizeof(char));
			lit[1] = '\0'; lit[2] = '\0';
			if (cur == '\\') {
				lit[0] = cur; lit[1] = nex;
				lit_regex = Regex_New_Escaped(lit);
				i++; //this moves i to nex, then eol will move i to after nex
			} else {
				lit[0] = cur;
				lit_regex = Regex_New_Direct(lit);
				//No need for special movement
			}

			bracket_count++;
			bracket_internals = (struct regex **)realloc(bracket_internals, bracket_count * sizeof(struct regex *));
			if (bracket_internals == NULL) Regex_Error("realloc error in brackets\n");
			bracket_internals[bracket_count-1] = lit_regex;
		}
	}
	
	//We've completed the bracket rule creation, now delineate the field
	bracket_internals = (struct regex **)realloc(bracket_internals, (bracket_count + 1) * sizeof(struct regex*));
	bracket_internals[bracket_count] = NULL; //there is no more
	
	def->attached_data = bracket_internals;
	def->match_function = Regex_Match_Brackets;
	return def;
}

struct regex* Regex_New_Sequence(char *seq) {
	if (strlen(seq) != 3) Regex_Error("New Sequence. Sequence given not len = 3\n");

	struct regex *def = (struct regex *)malloc(sizeof(struct regex));
	def->attached_data = seq;
	def->match_function = Regex_Match_Sequence;
	return def;
}

struct regex* Regex_New_Qualifier(struct regex *prev, char qualifier) {
	struct regex *def = (struct regex *)malloc(sizeof(struct regex));
	def->attached_data = malloc(2*sizeof(long long int));
	
	char *ptr_attch = (char *)malloc(sizeof(char));
	*ptr_attch = qualifier;
	((long long int *)def->attached_data)[0] = (long long int)ptr_attch;
	((long long int *)def->attached_data)[1] = (long long int)prev;

	def->match_function = Regex_Match_Qualifier;
	return def;
}

struct regex* Regex_New_Escaped(char *special) {
	if (strlen(special) != 2) Regex_Error("New Escaped. Not given len = 2\n");

	struct regex *def = (struct regex *)malloc(sizeof(struct regex));
	def->attached_data = special;
	def->match_function = Regex_Match_Escaped;
	return def;
}
