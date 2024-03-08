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

//Helper for Regex_New_Group
void add_to_grouping(struct regex ***grp, int *count, struct regex *reg) {
	*count += 1;
	*grp = realloc(*grp, *count * sizeof(struct regex *));
	if (grp == NULL) Regex_Error("Realloc error in grouping!");

	(*grp)[*count-1] = reg;
}

struct regex* Regex_New_Group(char *internals) {
	if (strlen(internals) <= 0) Regex_Error("New Group. Internals were empty?");

	int *count = calloc(1, sizeof(int));
	struct regex **grouping = NULL;

	struct regex *reg = malloc(sizeof(struct regex));
	reg->type = RT_GROUP;

	struct regex *ss = Regex_New_Escaped("\\s");

	char substr[100];
	char esc[2] = "a";
	size_t consume_pos = 0;
	enum regex_type mode = RT_DIRECT;
	for (size_t i = 0; i < strlen(internals); i++) {
		char cur = internals[i];

		esc[0] = cur;
		if (ss->match_function(esc, ss->attached_data)) Regex_Warning("Regex Definition contains whitespace. Consider using '\\s' instead.");

		if (mode == RT_DIRECT) {
			if (cur == ']') Regex_Error("Mismatched Right Bracket. Did you mean '\\]'?");
			if (cur == ')') Regex_Error("Mismatched Right Parenthesis. Did you mean '\\)?");
			
			if (cur == '(') {
				mode = RT_GROUP;
			} else if (cur == '\\') {
				mode = RT_ESCAPED;
			} else if (cur == '?' || cur == '*' || cur == '+') { //NOTE: we need a better way to detect this
				mode = RT_QUALIFIER;
			} else if (cur == '[') {
				mode = RT_BRACKETS;
			} else if (cur == '|') {
				mode == RT_OR;
			} else if (i == (strlen(internals)-1)) {
				mode = RT_UNDEFINED; //end of internals
			}

			if (consume_pos >= (i-1)) {
				if (mode == RT_QUALIFIER && *count == 0) Regex_Error("Missing Regex for Qualifier");
				if (mode == RT_OR && *count == 0) Regex_Error("Missing Regex for Or");
				consume_pos++;
			} else if (mode != RT_DIRECT) {
				int j = (mode == RT_UNDEFINED) ? i : i - 1;
				strncopy(substr, definition+consume_pos, j-consume_pos);
				substr[j-consume_pos+1] = '\0';
				
				struct regex *direct = Regex_New_Direct(substr);
				if (or_level <= 0) {
					add_to_group(&grouping, count, direct);
				} else {
					add_or_to_group(&grouping, count, direct);
					or_level--;
				}

				consume_pos = i+1;
			}
		} else if (mode == RT_GROUP) {
			if (cur == ')' && internals[i-1] != '\\') {
				strncopy(substr, internals+consume_pos, (i-1)-consume_pos);
				substr[i-consume_pos] = '\0';

				add_to_group(&grouping, count, Regex_New_Group(substr));

				mode = RT_DIRECT;
				consume_pos = i+1;
			} else if (i == (strlen(internals)-1)) {
				Regex_Error("Missing right parenthesis for group. Reached end of definition without right parenthesis ')'.");
			}
		} else if (mode == RT_ESCAPED) {
			substr[0] = '\\'; substr[1] = cur; substr[2] = '\0';

			add_to_group(&grouping, count, Regex_New_Escaped(substr));

			mode = RT_DIRECT;
			consume_pos = i+1;
		} else if (mode == RT_QUALIFIER) {
			struct regex *last = grouping[*count-1];
			if (last->type == RT_QUALIFIER) Regex_Error("Cannot have a qualified qualifier.");

			char qualifier = internals[i-1];
			if (last->type == RT_OR) {
				struct regex *or_item = ((struct regex **)last->attached_data)[1];
				if (or_item->type == RT_QUALIFIER) Regex_Error("Cannot have a qualified qualifier.");
				struct regex *qualified = Regex_New_Qualifier(or_item, qualifier);
				((struct regex **)last->attached_data)[1] = qualified;
			} else {
				struct regex *qualified = Regex_New_Qualifier(last, qualifier);
				grouping[*count-1] = qualified;
			}

			mode = RT_DIRECT;
			consume_pos = i;
			i--; //for loop gonna push it forward, this brings that back
		} else if (mode == RT_BRACKETS) {
			if (cur == ']' && internals[i-1] != '\\') {
				strncopy(substr, definition+consume_pos, (i-1)-consume_pos);
				substr[i-consume_pos] = '\0';

				add_to_group(&grouping, count, Regex_New_Brackets(substr));

				mode = RT_DIRECT;
				consume_pos = i+1;
			} else if (i == (strlen(internals)-1)) {
				Regex_Error("Missing right bracket for bracket-group. Reached end of definition without right bracket ']'.");
			}
		} else if (mode == RT_OR) {
			int len = strlen(internals) - consume_pos;
			if (len <= 0) Regex_Error("Cannot Or with nothing. '|' at end of group/definition.");

			strncopy(substr, internals+consume_pos, len);
			substr[len+1] = '\0';

			struct regex *all_prev = malloc(sizeof(struct regex));
			all_prev->type = RT_GROUP;
			all_prev->attached_data = malloc(2*sizeof(long long int));
			all_prev->match_function = Regex_Match_Group;
			
			((long long int *)all_prev->attached_data)[0] = (long long int)count;
			((long long int *)all_prev->attached_data)[1] = (long long int)grouping;

			struct regex *all_aftr = Regex_New_Group(substr);

			count = malloc(sizeof(int)); *count = 1;
			grouping = malloc(sizeof(struct regex *)); grouping[0] = Regex_New_Or(all_prev, all_aftr);
			
			break; //We've consumed everything
		}
	}

	Regex_Free(ss); ss = NULL; //Good memory management
	
	if (*count == 0) Regex_Error("New Group. Internal error, was not able to produce a single regex?");
	
	if (*count == 1) {
		struct regex *only = grouping[0];
		free(grouping);
		free(count);
		free(reg);
		return only;
	}
	
	reg->match_function = Regex_Match_Group;
	reg->attached_data = malloc(2*sizeof(long long int));
	((long long int *)reg->attached_data)[0] = (long long int)count;
	((long long int *)reg->attached_data)[1] = (long long int)grouping;
	
	return reg;
}
