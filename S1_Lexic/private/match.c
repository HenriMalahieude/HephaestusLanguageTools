#include <string.h>
#include <stdio.h> //while printf is being used
#include <ctype.h>

#include "../lexic.h"
#include "regex.h"

enum regex_type {
	RT_UNDEFINED,
	RT_DIRECT, 
	RT_ESCAPED,
	RT_OR,
	RT_BRACKETS,
	RT_QUALIFIER,
	RT_CGROUP,
};

bool escaped_match(char esc, char in) {
	if (esc == 's') return isspace(in);
	if (esc == 'S') return !isspace(in);
	if (esc == 'd') return in >= (int)'0' && in <= (int)'9';
	if (esc == 'D') return in < (int)'0' || in > (int)'9';
	if (esc == '0') return in == '\0'; //dunno if this is really useful
	if (esc == 'n') return in == '\n';
	if (esc == 't') return in == '\t';
	if (esc == 'r') return in == '\r';

	return esc == in;
}

bool is_qualifier(char q) {
	return q == '*' || q == '?' || q == '+';
}

//If failed escaped, DO NOT move past \ pls, let this function handle it
//returns whether or not it recovered properly
bool recover_failed_qualifier(char *reg, size_t *ri, bool *plus_match) {
	if (*ri >= strlen(reg)) Lexic_Error("_recover_failed_qualifier. Regex Index already at end of str?");

	char cur = reg[*ri];
	if (is_qualifier(cur)) Lexic_Error("_recover_failed_qualifier. Incorrect use of this function on qualifier?");
	
	size_t q_ind = (cur == '\\') ? *ri+2 : *ri+1;
	char qualifier = reg[q_ind];
	if (qualifier == '+' && !(*plus_match)) return false;
	
	Lexic_Warn("Recovering from failed qualifier", LWT_DEBUG);

	*plus_match = false;
	*ri = q_ind+1; //skip past this modifier
	return true;
}

//regex control take over ahead?
enum regex_type get_ctrl_ahead(char *reg, size_t reg_ind, size_t *ind, char *mod) {
	int paren_lvl = 0;
	for (size_t i = reg_ind+1; i <= strlen(reg); i++) {
		char prv = (i > 0) ? reg[i-1] : '\0';
		*mod = reg[i];
		*ind = i;

		if (is_qualifier(*mod)) return RT_QUALIFIER;

		if (prv != '\\' && *mod == '|') return RT_OR;

		if (prv != '\\' && *mod == '(') paren_lvl++;
		if (prv != '\\' && *mod == ')' && paren_lvl == 0) return RT_CGROUP;
		if (prv != '\\' && *mod == ')' && paren_lvl > 0) paren_lvl--;
	}

	return RT_DIRECT; //NOTE: we only want modifiers that take control, escaped doesn't, nor does brackets
}

//places regex index on matching right parenthesis of this cgroup
void forward_cgroup(char *reg, size_t *ri) {
	int paren_lvl = 0;
	while (*ri < strlen(reg)) {
		char prv = (*ri > 0) ? reg[*ri-1] : '\0';
		char cur = reg[*ri];
		if (prv != '\\') {
			if (cur == '(') paren_lvl++;
			else if (cur == ')' && paren_lvl == 0) return;
			else if (cur == ')') {
				paren_lvl--;
				if (paren_lvl < 0) Lexic_Error("_forward_cgroup. Missing Right Parenthesis for capture group?");
			}
		}
		(*ri)++;
	}
	
	Lexic_Error("_forward_cgroup. Could not leave capture group?");
}

//expects ri to be at ) of cgroup we are attempting to restart
void restart_cgroup(char *reg, size_t *ri) {
	*ri -= 1; //move before )
	int paren_lvl = 0;
	while (*ri > 0) { //travelling backwards
		char prv = (*ri > 0) ? reg[*ri-1] : '\0';
		char cur = reg[*ri];
		if (prv != '\\') {
			if (cur == ')') paren_lvl++;
			else if (cur == '(' && paren_lvl == 0) return;
			else if (cur == '(') {
				paren_lvl--;
				if (paren_lvl < 0) Lexic_Error("_restart_cgroup. Missing Left Parenthesis for capture group?");
			}
		}

		(*ri)--;
	}

	Lexic_Error("_restart_cgroup. Could not restart capture group?");
}

bool Regex_Match(char *reg, char *input) {
	size_t rlen = strlen(reg);
	size_t ilen = strlen(input);

	bool plus_quali = false;
	bool lst_grp_fail = false;
	int grp_lvl = 0; //capture group recursion lvl
	size_t ii = 0; //Input Index
	size_t ri = 0; //Regex Index

	enum regex_type mode = RT_UNDEFINED;
	while (ii <= ilen && ri < rlen) { //NOTE: <= for if we need to catch ?'s at the end
		char prv = (ri > 0) ? reg[ri-1] : '\0';
		char cur = reg[ri];
		char nxt = reg[ri+1];

		//printf("ii: %lld, ilen: %lld, ri: %lld, rlen: %lld\n", ii, ilen, ri, rlen);

		if (mode == RT_UNDEFINED) {
			if (cur == '\\') {mode = RT_ESCAPED;}
			else if (cur == '|') {mode = RT_OR;}
			else if (cur == '[') {mode = RT_BRACKETS;}
			else if (is_qualifier(cur)) {mode = RT_QUALIFIER;}
			else if (cur == '(' || cur == ')') {mode = RT_CGROUP;}
			else {mode = RT_DIRECT;} //the default mode
		} else if (mode == RT_DIRECT) {
			if (cur == input[ii] || (cur == '.' && input[ii] != '\n')) {
				ii++; //consume character
				ri++; //onto nxt regex
			} else {
				if (is_qualifier(nxt)) {
					if (!recover_failed_qualifier(reg, &ri, &plus_quali)) break;
				} else {
					if (grp_lvl <= 0) break;
					lst_grp_fail = true;
					forward_cgroup(reg, &ri); 
				}
			}
			mode = RT_UNDEFINED;
		} else if (mode == RT_ESCAPED) {
			//printf("escaped\n");
			if (escaped_match(nxt, input[ii])) {
				ii++;
				ri+=2; //skip \\ and escaped char
			} else {
				if (is_qualifier(reg[ri+2])) {
					if (!recover_failed_qualifier(reg, &ri, &plus_quali)) break;
				} else {
					if (grp_lvl <= 0) break;
					lst_grp_fail = true;
					forward_cgroup(reg, &ri);
				}
			}
			mode = RT_UNDEFINED;
		} else if (mode == RT_OR) { //if we reached this mode normally, that means we've successfully matched everything before it
			if (grp_lvl <= 0) return ii >= ilen;
			lst_grp_fail = false;
			forward_cgroup(reg, &ri);
			mode = RT_UNDEFINED;
		} else if (mode == RT_BRACKETS) {
			Lexic_Warn("Match. In brackets mode!", LWT_DEBUG);
			char matching = input[ii];
			bool match = false;
			ri++; //Assuming that we started at '[' of the brackets
			while (ri < rlen) {
				char bcur = reg[ri];

				char str[] = "Checking _ vs _"; str[9] = matching; str[14] = bcur;
				Lexic_Warn(str, LWT_DEBUG);

				if (bcur == ']') {ri++; break;}
				if (bcur == '\\') {
					ri++;
					match = escaped_match(reg[ri], matching);
				} else if (bcur == '-' && reg[ri-1] != '\\') {
					Lexic_Warn("Match. Brackets mode reached a sequence!", LWT_DEBUG);
					if (reg[ri-2] == '\\' || reg[ri+1] == '\\') Lexic_Error("Match. This lexical analyzer does not support escaped characters in sequences.");
					match = (int)matching >= (int)reg[ri-1] && (int)matching <= (int)reg[ri+1];
					ri++; //skip the sequence we've already evaluated
				} else {
					match = (matching == bcur);
				}

				if (match) {
					Lexic_Warn("Match. Found a match leaving brackets!", LWT_DEBUG);
					break;
				}
				
				ri++;
			}

			if (match) {
				ii++;
				while (ri < rlen) {
					if (reg[ri] == ']') {ri++; break;}
					ri++;
				}
			} else {
				if (is_qualifier(reg[ri])) {
					ri--;
					if (!recover_failed_qualifier(reg, &ri, &plus_quali)) break;
				} else {
					if (grp_lvl <= 0) break;
					lst_grp_fail = true;
					forward_cgroup(reg, &ri);
				}
			}

			mode = RT_UNDEFINED;
		} else if (mode == RT_QUALIFIER) {
			if (prv == '\\') Lexic_Error("Match. Cannot enter qualifier mode if previous is escaped, what?");
			if (cur == '?') {
				ri++;
				mode = RT_UNDEFINED;
				continue;
			}

			plus_quali = true;
			if (prv == ')' && reg[ri-2] != '\\') {
				restart_cgroup(reg, &ri);
			} else if (prv == ']' && reg[ri-2] != '\\') {
				while (ri > 0) {
					if (reg[ri] == '[' && reg[ri-1] != '\\') break;
					ri--;
				}
			} else {
				if (reg[ri-2] == '\\') ri--;
				ri--;
			}

			mode = RT_UNDEFINED;
		} else if (mode == RT_CGROUP) {
			printf("cgroup unimplemented\n");
			return false;
			mode = RT_UNDEFINED;
		}
	}

	//printf("Match end: %d >= %d && %d >= %d\n", ii, ilen, ri, rlen);
	return ii >= ilen && ri >= rlen;
}
