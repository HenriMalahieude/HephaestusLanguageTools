#include <stdlib.h>
#include <stdio.h>
#include "regex.h"
#include "match.h"

void Regex_Error(char *msg) {
	printf("Regex Creation Error @ L%d, C%d: %s", line_no, colu_no, msg);
	exit(1);
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
	struct regex *def = (struct regex*)malloc(sizeof(struct regex));

	struct regex **bracket_internals;
	int bracket_count = 0;

	for (size_t i = 0; i < strlen(internals); i++) {
		char cur = internals[i];
		//TODO
	}
}

struct regex* Regex_New_Sequence(char *seq);

struct regex* Regex_New_Qualifier(struct regex *prev);
