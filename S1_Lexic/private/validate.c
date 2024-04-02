#include <string.h>
#include "../lexic.h"
#include "regex.h"
#include "lexic_internal.h"
#include "warn.h"

//Takes control of reg_colu_no
bool Regex_Validate(char *reg) {
	int paren_count = 0;
	int bracket_count = 0;
	for (size_t i = 0; i < strlen(reg); i++) {
		char prv = (i > 0) ? reg[i-1] : '\0';
		char cur = reg[i];
		char nxt = reg[i+1];

		if (cur == '|' && (i == 0 || prv == '|' || prv == '(' || prv == ')' || nxt == '\0')) {
			Lexic_Warn("Validate. This lexical analyzer does not support empty/0-length or expressions.", LWT_MJRWRN);
			return false;
		} else (cur == '|' && (prv == '*'  || prv == '?')) {
			Lexic_Warn("Validate. This or statement may never evaluate due to '*' or '?' qualifier.", LWT_STDWRN);
		}

		if (cur == '[' && prv != '\\') { 
			bracket_count++; 
			if (bracket_count > 1) {
				Lexic_Warn("Validate. This lexical analyzer does not support nested brackets. Did you mean '\\['?", LWT_MJRWRN);
				return false;
			}
		} else if (cur == ']' && prv != '\\') {
			if (prv == '[') {
				Lexic_Warn("Validate. This lexical analyzer does not support empty/0-length expressions. Like '[]'.", LWT_MJRWRN);
				return false;
			}
		
			bracket_count--;
			
			if (bracket_count < 0) {
				Lexic_Warn("Validate. Regex did not properly begin a brackets expression. Missing Left Bracket '['!", LWT_MJRWRN);
				return false;
			}
		}

		if (cur == '(' && prv != '\\') paren_count++;
		else if (cur == ')' && prv != '\\') {
			if (prv == '(') {
				Lexic_Warn("Validate. This lexical analyzer does not support empty/0-length expressions. Like '()'.", LWT_MJRWRN);
				return false;
			}

			paren_count--;

			if (paren_count < 0) {
				Lexic_Warn("Validate. Regex did not properly begin a group structure. Missing Left Parenthesis '('!", LWT_MJRWRN);
				return false;
			}
		}

		if (prv != '\\' && (cur == '*' || cur == '?' || cur == '+')) {
			if (i == 0) {
				Lexic_Warn("Validate. Cannot qualify empty/0-length string.", LWT_MJRWRN);
				return false;
			}

			if (prv == '*' || prv == '?' || prv == '+') {
				Lexic_Warn("Validate. This lexical analyzer does not support qualified qualifiers.", LWT_MJRWRN);
				return false;
			}
			
			if (prv == '|') {
				Lexic_Warn("Validate. Cannot qualify empty/0-length string (on or).", LWT_MJRWRN);
				return false;
			}
		}

		if (bracket_count > 0 && cur == '-') {
			if ((int)prv > (int)nxt) Lexic_Warn("Validate. Note that this sequence within brackets will exclude range specified.", LWT_VERBSE);
			else if (prv == nxt) Lexic_Warn("Validate. Sequence within brackets has no range.", LWT_STDWRN);
		}
		regex_colu_no++;
	}

	if (paren_count != 0) {
		Lexic_Warn("Validate. Regex did not properly end a group structure. Missing Right Parenthesis ')'!", LWT_MJRWRN);
		return false;
	}
	if (bracket_count > 0) {
		Lexic_Warn("Validate. Regex did not properly end a brackets expression. Missing Right Bracket ']'!", LWT_MJRWRN);
		return false;
	}

	return true;
}
