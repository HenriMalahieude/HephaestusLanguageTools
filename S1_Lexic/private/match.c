#include <string.h>
#include <stdio.h>
#include "regex.h"
#include "match.h"

bool Regex_Match_Direct(char *input, void *attached_data) {
	char *direct = (char*)attached_data;
	return strcmp(input, direct) == 0;
}

bool Regex_Match_Or(char *input, void *attached_data) {
	struct regex *left = ((struct regex**)attached_data)[0];
	struct regex *right = ((struct regex**)attached_data)[1];

	bool L = left->match_function(input, left->attached_data);
	bool R = false;
	
	//Short-Circuiting is a thing
	if (!L) R = right->match_function(input, right->attached_data);
	return L || R;
}

bool Regex_Match_Brackets(char *input, void *attached_data) {
	if (strlen(input) != 1) return false;

	struct regex** list = (struct regex**)attached_data; //NULL-Terminated
	for (struct regex** cur = list; *cur != NULL; cur++) {
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
	long long int q_ptr = ((long long int*)attached_data)[0]; //Yes, I am well aware of how dastardly this is
	long long int g_ptr = ((long long int*)attached_data)[1];

	char qualifier = *((char *)q_ptr);
	struct regex *group = (struct regex *)g_ptr;

	unsigned int match_count = 0; //For ? and +
	size_t consume_count = 0; //For if we have "[abc]+" and we receive "aacbabcd", that last d will mean this doesn't match completely

	size_t start = 0; //Start -> End: Inclusive -> Exclusive
	char substring[1000]; size_t subs_len = 0;
	for (size_t end = 1; end <= strlen(input); end++) { //"<=" since end is exclusive
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

bool Regex_Match_Escaped(char *input, void *attached_data) {
	if (strlen(input) != 1) return false;

	char *spec = (char*)attached_data;

	if (spec[0] != '\\') Regex_Error("Match Escaped. Wasn't escaped?");

	bool white_space = (
		input[0] == (char)32 || //space
		((int)input[0] >= 9 && (int)input[0] <= 13) //h-vtab, nline, npage, carret
	);

	if (spec[1] == '.') return input[0] == '.';
	if (spec[1] == 'n') return input[0] == '\n';
	if (spec[1] == 's') return white_space;
	if (spec[1] == 'S') return !white_space;

	return false;
}
