#include <string.h>
#include "match.h"

bool Regex_Match_Direct(char *input, void *attached_data) {
	char *direct = (char*)attached_data;
	return strcmp(input, direct) == 0;
}

bool Regex_Match_Or(char *input, void *attached_data) {
	struct regex *left = ((struct regex**)att)[0];
	struct regex *right = ((struct regex**)att)[1];

	bool L = left->match_function(input, left->attached_data);
	bool R = false;
	
	//Short-Circuiting is a thing
	if (!L) R = right->match_function(input, right->attached_data);
	return L || R;
}

bool Regex_Match_Brackets(char *input, void *attached_data);

bool Regex_Match_Sequence(char *input, void *attached_data);

bool Regex_Match_Qualifier(char *input, void *attached_data);
