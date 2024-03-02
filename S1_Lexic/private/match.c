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

bool Regex_Match_Brackets(char *input, void *attached_data) {
	if (strlen(input) != 1) return false;

	struct regex** list = (struct regex**)attached_data; //NULL-Terminated
	for (struct regex** cur = list; cur != NULL; cur++) {
		struct regex *using = cur[0];
		bool value = using->match_function(input, using->attached_data);
		if (value) return true;
	}

	return false;
}

bool Regex_Match_Sequence(char *input, void *attached_data) {
	if (strlen(input) != 1) return false; //This seems like a bad idea, but since only in brackets..?
	int ascii_input = (int)*input;

	char *seq = (char*)attached_data;
	int from = (int)seq[0]; 
	int to   = (int)seq[2]; // ie: a-z, index 1 is '-'

	if (from >= to) Regex_Error("Match Sequence 'from' >= 'to'");
	
	return ascii_input >= from && ascii_input <= to;
}

//Qualifiers Implemented: ?, +, *,
bool Regex_Match_Qualifier(char *input, void *attached_data) { //This will most likely need most of the testing
	char qualifier = *(char*)((long long int**)attached_data)[0]; //Yes, I am well aware of how dastardly this is
	struct regex *group = *(struct regex**)((long long int**)attached_data)[1];

	unsigned int match_count = 0; //For ? and +
	int consume_count = 0; //For if we have "[abc]+" and we receive "aacbabcd", that last d will mean this doesn't match completely

	int start = 0; //Start -> End: Inclusive -> Exclusive
	char substring[1000]; int subs_len = 0;
	for (int end = 1; end <= strlen(input); end++) { //"<=" since end is exclusive
		if (start >= strlen(input)) break; //No reason for it to be, but never know
		
		//forward the substring
		if (start + subs_len < strlen(input)) {
			substring[subs_len] = input[start+subs_len]; //next char
			substring[subs_len+1] = '\0'; //end string
			subs_len++; //inc size
		}
		
		bool matched = group->match_function(substring, group->attached_data);
		if (matched) { //"consume"
			match_count++;
			start = end;
			consume_count += subs_len;
			subs_len = 0;
		}
	}

	if (consume_count < strlen(input)) return false; 

	if (qualifier == '?') return match_count <= 1;
	if (qualifier == '*') return true; //doesn't matter for zero or more
	if (qualifier == '+') return match_count >= 1;

	return false;
}
