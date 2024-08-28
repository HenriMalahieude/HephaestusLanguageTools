#include <string.h>

#include "lexic_internal.h"
#include "regex.h"
#include "../../helpers/log/warn.h"

//Takes control of regex_colu_no
bool RegexValidate(char *reg) {
	int paren_count = 0;
	int bracket_count = 0;
	for (size_t i = 0; i < strlen(reg); i++) {
		char prv = (i > 0) ? reg[i-1] : '\0';
		char cur = reg[i];
		char nxt = reg[i+1];

		if (cur == '|') { //or mode
			if (i == 0 || prv == '|' || prv == '(' || nxt == '\0') {
				HLTWarn("Validate. This lexical analyzer does not support empty/0-length or expressions.", regex_line_no, regex_colu_no, HLT_MJRWRN);
				return false;
			} else if (prv == '*' || prv == '?') {
				HLTWarn("Validate. This or statement may never evaluate due to '*' or '?' qualifier.", regex_line_no, regex_colu_no, HLT_STDWRN);
			}
		}


		if (cur == '[' && prv != '\\') { 
			bracket_count++; 
			if (bracket_count > 1) {
				HLTWarn("Validate. This lexical analyzer does not support nested brackets. Did you mean '\\['?", regex_line_no, regex_colu_no, HLT_MJRWRN);
				return false;
			}
		} else if (cur == ']' && prv != '\\') {
			if (prv == '[') {
				HLTWarn("Validate. This lexical analyzer does not support empty/0-length expressions. Like '[]'.", regex_line_no, regex_colu_no, HLT_MJRWRN);
				return false;
			}
		
			bracket_count--;
			
			if (bracket_count < 0) {
				HLTWarn("Validate. Regex did not properly begin a brackets expression. Missing Left Bracket '['!", regex_line_no, regex_colu_no, HLT_MJRWRN);
				return false;
			}
		}

		if (cur == '(' && prv != '\\') { 
			paren_count++;
		} else if (cur == ')' && prv != '\\') {
			if (prv == '(') {
				HLTWarn("Validate. This lexical analyzer does not support empty/0-length expressions. Like '()'.", regex_line_no, regex_colu_no, HLT_MJRWRN);
				return false;
			}

			paren_count--;

			if (paren_count < 0) {
				HLTWarn("Validate. Regex did not properly begin a group structure. Missing Left Parenthesis '('!", regex_line_no, regex_colu_no, HLT_MJRWRN);
				return false;
			}
		}

		if (prv != '\\' && (cur == '*' || cur == '?' || cur == '+')) {
			if (i == 0) {
				HLTWarn("Validate. Cannot qualify empty/0-length string.", regex_line_no, regex_colu_no, HLT_MJRWRN);
				return false;
			}

			if (prv == '*' || prv == '?' || prv == '+') {
				HLTWarn("Validate. This lexical analyzer does not support qualified qualifiers.", regex_line_no, regex_colu_no, HLT_MJRWRN);
				return false;
			}
			
			if (prv == '|') {
				HLTWarn("Validate. Cannot qualify empty/0-length string (on or).", regex_line_no, regex_colu_no, HLT_MJRWRN);
				return false;
			}
		}

		if (bracket_count > 0 && cur == '-') {
			if ((int)prv > (int)nxt) HLTWarn("Validate. Note that this sequence within brackets will exclude range specified.", regex_line_no, regex_colu_no, HLT_VERBSE);
			else if (prv == nxt) HLTWarn("Validate. Sequence within brackets has no range.", regex_line_no, regex_colu_no, HLT_STDWRN);
		}
		regex_colu_no++;
	}

	if (paren_count != 0) {
		HLTWarn("Validate. Regex did not properly end a group structure. Missing Right Parenthesis ')'!", regex_line_no, regex_colu_no, HLT_MJRWRN);
		return false;
	}
	if (bracket_count > 0) {
		HLTWarn("Validate. Regex did not properly end a brackets expression. Missing Right Bracket ']'!", regex_line_no, regex_colu_no, HLT_MJRWRN);
		return false;
	}

	return true;
}
