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
	struct regex *reg = (struct regex*)malloc(sizeof(struct regex));
	reg->type = RT_DIRECT;

	char *local_copy = (char *)malloc((strlen(match)+1) * sizeof(char));
	strcpy(local_copy, match);

	reg->attached_data = local_copy;
	reg->match_function = Regex_Match_Direct;

	return reg;
}

//Assuming left and right are dynamically created with malloc, but they should be
struct regex* Regex_New_Or(struct regex *left, struct regex *right) {
	struct regex *reg = (struct regex*)malloc(sizeof(struct regex));
	reg->type = RT_OR;
 	
	void *att = malloc(2*sizeof(struct regex*));
	((struct regex**)att)[0] = left;
	((struct regex**)att)[1] = right;
	reg->attached_data = att;
	reg->match_function = Regex_Match_Or;

	return reg;
}

struct regex* Regex_New_Brackets(char *internals) {
	if (strlen(internals) <= 0) Regex_Error("New Brackets. internals cannot be len = 0");
	struct regex *reg = (struct regex*)malloc(sizeof(struct regex));
	reg->type = RT_BRACKETS;
	
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
			if (bracket_internals == NULL) Regex_Error("realloc error in brackets");
			bracket_internals[bracket_count-1] = seq_regex;

			i += 2; //move i to i+2, on last char of seq. then eol will move i to after seq
		} else if (cur != '\0') { //This is a "literal"
			if (cur == '[' || cur == ']') Regex_Error("Bracket Rule within Bracket Rule not supported");

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
			if (bracket_internals == NULL) Regex_Error("realloc error in brackets");
			bracket_internals[bracket_count-1] = lit_regex;
		}
	}
	
	//We've completed the bracket rule creation, now delineate the field
	bracket_internals = (struct regex **)realloc(bracket_internals, (bracket_count + 1) * sizeof(struct regex*));
	bracket_internals[bracket_count] = NULL; //there is no more
	
	reg->attached_data = bracket_internals;
	reg->match_function = Regex_Match_Brackets;
	return reg;
}

struct regex* Regex_New_Sequence(char *seq) {
	if (strlen(seq) != 3) Regex_Error("New Sequence. Sequence given not len = 3");

	char *local_copy = (char *)malloc(4 * sizeof(char));
	strcpy(local_copy, seq);

	struct regex *reg = (struct regex *)malloc(sizeof(struct regex));
	reg->type = RT_SEQUENCE;
	reg->attached_data = local_copy;
	reg->match_function = Regex_Match_Sequence;
	return reg;
}

struct regex* Regex_New_Qualifier(struct regex *prev, char qualifier) {
	struct regex *reg = (struct regex *)malloc(sizeof(struct regex));
	reg->type = RT_QUALIFIER;
	reg->attached_data = malloc(2*sizeof(long long int));
	
	char *ptr_attch = (char *)malloc(sizeof(char));
	*ptr_attch = qualifier;
	((long long int *)reg->attached_data)[0] = (long long int)ptr_attch;
	((long long int *)reg->attached_data)[1] = (long long int)prev;

	reg->match_function = Regex_Match_Qualifier;
	return reg;
}

struct regex* Regex_New_Escaped(char *special) {
	if (strlen(special) != 2) Regex_Error("New Escaped. Not given len = 2");

	char *local_copy = (char *)malloc(3*sizeof(char));
	strcpy(local_copy, special);

	struct regex *reg = (struct regex *)malloc(sizeof(struct regex));
	reg->type = RT_ESCAPED;
	reg->attached_data = local_copy;
	reg->match_function = Regex_Match_Escaped;
	return reg;
}
