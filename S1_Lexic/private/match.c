#include <string.h>
#include <stdio.h>
#include "regex.h"

#define MATCH_FUNC(name) bool name(char *input, void *attached_data)

MATCH_FUNC(Regex_Match_Direct) {
	return strcmp(input, (char *)attached_data) == 0;
}

MATCH_FUNC(Regex_Match_Or) {
	struct regex left = ((struct regex*)attached_data)[0];
	struct regex right = ((struct regex*)attached_data)[1];

	bool L = Regex_Match(left, input); 
	bool R = false;
	
	//Short-Circuiting is a thing
	if (!L) R = Regex_Match(right, input);
	return L || R;
}

MATCH_FUNC(Regex_Match_Brackets) {
	if (strlen(input) != 1) return false; //only consumes one character
	
	int blen = ((unsigned long long *)attached_data)[0];
	struct regex *brkts = ((unsigned long long *)attached_data)[1];

	for (int i = 0; i < blen; i++) {
		struct regex using = brkts[i];
		if (Regex_Match(input, using)) return true;
	}

	return false;
}

MATCH_FUNC(Regex_Match_Sequence) {
	if (strlen(input) != 1) return false; //only consumes one character

	int ascii_input = (int)*input;

	char *seq = (char*)attached_data;
	int from = (int)seq[0]; 
	int to   = (int)seq[2]; // ie: a-z, index 1 is '-'

	if (from >= to) Regex_Error("Match Sequence. Sequence 'from' >= 'to'?");
	
	return ascii_input >= from && ascii_input <= to;
}

//Qualifiers Implemented: ?, +, *,
MATCH_FUNC(Regex_Match_Qualifier) { 
	char qualifier = ((unsigned long long *)reg.attached_data)[0];
	struct regex *reg = ((unsigned long long *)reg.attached_data)[1];

	unsigned int match_count = 0; 
	size_t consume_count = 0; 

	size_t start = 0; 
	char substring[100]; size_t subs_len = 0;
	for (size_t end = 1; end <= strlen(input); end++) { //"<=" since end is exclusive
		if (start >= strlen(input)) break; //No reason for it to be, but never know
		
		//forward the substring
		if (start + subs_len < strlen(input)) {
			substring[subs_len] = input[start+subs_len]; //next char
			substring[subs_len+1] = '\0'; //end string
			subs_len++; //inc size
		}
		
		if (Regex_Match(*reg, substring)) { //"consume"
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

	if (spec[1] == '-') return input[0] == '-';
	if (spec[1] == '.') return input[0] == '.';
	if (spec[1] == 'n') return input[0] == '\n';
	if (spec[1] == 's') return white_space;
	if (spec[1] == 'S') return !white_space;
	if (spec[1] == 't') return input[0] == '\t';
	if (spec[1] == '[') return input[0] == '[';
	if (spec[1] == ']') return input[0] == ']';
	if (spec[1] == '?') return input[0] == '?';
	if (spec[1] == '*') return input[0] == '*';
	if (spec[1] == '+') return input[0] == '+';
	if (spec[1] == '\\') return input[0] == '\\'; //I really should make this more dynamic and less hardcoded dear god

	Regex_Error("Escaped character not valid from definition not valid.");

	return false;
}

MATCH_FUNC(Regex_Match_Group) {
	//TODO
}

//Defined in regex.h
bool Regex_Match(struct regex reg, char *input) {
	switch (reg.type) {
		case RT_DIRECT:
			return Regex_Match_Direct(input, reg.attached_data);
		case RT_OR:
			return Regex_Match_Or(input, reg.attached_data);
		case RT_BRACKETS:
			return Regex_Match_Brackets(input, reg.attached_data);
		case RT_SEQUENCE:
			return Regex_Match_Sequence(input, reg.attached_data);
		case RT_QUALIFIER:
			return Regex_Match_Qualifier(input, reg.attached_data);
		case RT_ESCAPED:
			return Regex_Match_Escaped(input, reg.attached_data);
		case RT_GROUP:
			return Regex_Match_Group(input, reg.attached_data);
		case RT_UNDEFINED:
			Regex_Error("Match. Attempted to match an UNDEFINED regex?");
			break;
		default:
			Regex_Error("Match. Fell through switch statement?");
			break;
	}

	Regex_Error("Match. Escaped switch statement without return?");

	return false;
}
