#include <stdlib.h>
#include <string.h>

#include "syntac_internal.h"
#include "../../helpers/string/handy.h"
#include "../../helpers/log/warn.h"

//Allocate an Empty Rule Book
SyntacBook * SyntacBookAllocate() {
	SyntacBook *book = (SyntacBook*)malloc(sizeof(struct stc_book));

	if (book == NULL) HLT_ERR("Attempted malloc of a book failed?");
	
	book->rules = NULL;
	book->rule_count = 0;

	return book;
}

//Free a rule book (assumes everything within the rulebook and rules is dynamically allocated)
void SyntacBookFree(SyntacBook *book) {
	if (book == NULL) {
		HLT_WRN("Supplied book pointer was null?", HLT_MJRWRN);
		return;
	}

	for (int i = 0; i< book->rule_count; i++){
		struct stc_rule *rule = &book->rules[i];
		if (rule == NULL) {
			HLT_WRN("Rule pointer was null?", HLT_STDWRN);
			continue;
		}

		free(rule->name); rule->name = NULL;
		for (int j = 0; rule->elements[j] != NULL; j++) {
			free(rule->elements[j]); //free all elements inside
		}
		free(rule->elements); rule->elements = NULL; //free container
		
		//Free the sets we allocated
		if (rule->first_set != NULL) SetFree(rule->first_set);
		if (rule->follow_set != NULL) SetFree(rule->follow_set);
	}

	if (book->rules != NULL) free(book->rules); 
	book->rules = NULL; book->rule_count = 0;
	free(book);

	HLT_WRN("TODO: top and bottom table freeing!", HLT_MJRWRN);
}	

void SyntacBookRuleAdd(SyntacBook *book, char *left, char *right) {
	if (book == NULL) {
		HLT_WRN("Supplied book pointer was null?", HLT_MJRWRN);
		return;
	}

	if (left == NULL || left[0] == '\0') {
		HLT_WRN("Supplied 'left' of rule was null/empty?", HLT_MJRWRN);
		return;
	}
	
	//NOTE: Removed because we want null rules (epsilon rules iirc?)
	//if (right == NULL) HLT_WRN("Supplied 'right' of rule was null/empty?", HLT_VERBSE);

	//Count Amount of Elements (delimited by RIGHT_DELIM)
	int elm_cnt = 1;
	for (size_t i = 0; i < strlen(right); i++) if (right[i] == RIGHT_DELIM) elm_cnt++;

	//Initialize the rule + name
	struct stc_rule rule; 
	rule.name = NULL;
	rule.first_set = NULL;
	rule.follow_set = NULL;
	if (!trim(left, &rule.name)) {
		HLT_WRN("Attempted to insert rule with empty left/name?", HLT_MJRWRN);
		return;
	}
	
	//Allocate amount of elements detected
	rule.elements = (char**)calloc((elm_cnt+1), sizeof(char *));//calloc nulls out entire thing

	//Parse the elements now
	int elm_i = 0;
	size_t nconsumeIdx = 0;
	char substr[100];
	for (size_t i = 1; i <= strlen(right); i++) {
		if (right[i] == RIGHT_DELIM || right[i] == '\0') { //delimiter or end
			int len = (i - nconsumeIdx); //non-inclusive
			if (len-1 <= 0) {
				HLT_WRN("Empty element inside of rule's right production? Skipping.", HLT_VERBSE);
				continue;
			}

			strncpy(substr, right + nconsumeIdx, len);
			substr[len] = '\0';
			if (!trim(substr, &rule.elements[elm_i])) {
				HLT_WRN("Element was only whitespace? Skipping.", HLT_VERBSE);
				//strcpy(rule.elements[elm_i], substr);
			}

			elm_i += 1;
			nconsumeIdx = i+1; //skip delim
		}
	}

	if (elm_i < elm_cnt) {
		snprintf(substr, 100, "Rulebook attempted to add '%s' (%d prev) expected %d elements, got %d?", left, book->rule_count, elm_cnt, elm_i);
		HLT_WRN(substr, HLT_VERBSE);
	}

	//Place into the book, unless you have a better idea, we will be reallocing 1 by 1
	book->rules = (struct stc_rule *)realloc(book->rules, sizeof(struct stc_rule) * ++(book->rule_count)); 
	book->rules[book->rule_count-1] = rule;
}

SyntacBook * SyntacBookFromFile(char *file_name){
	if (file_name == NULL || file_name[0] == '\0') {
		HLT_WRN("Supplied file_name was null/empty?", HLT_MJRWRN);
		return NULL;
	}

	char *cntnts = ftostr(file_name);
	if (cntnts == NULL) {
		HLT_WRN("Could not convert file to string?", HLT_MJRWRN);
		return NULL;
	}

	int nl = 0; //new line location
	for (nl = 0; cntnts[nl] != '\0' && cntnts[nl] != '\n'; nl++);
	if (nl == 0) HLT_ERR("File had no type specifier? (Or was empty?)");

	char substr[100];
	strncpy(substr, cntnts, nl);

	enum stc_parsing_style type = STC_NON;
	if 	(strncmp(substr, "TOP", 3) == 0) type = STC_TOP;
	else if (strncmp(substr, "BOT", 3) == 0) type = STC_BOT;
	else {
		HLT_WRN("File did not contain proper parsing type?", HLT_MJRWRN);
		return NULL;
	}
		
	SyntacBook *book = SyntacBookFromString(cntnts + nl + 1, type); //past the new line
	
	free(cntnts);
	return book;
}

SyntacBook * SyntacBookFromString(char *stream, SyntacTreeType type) {
	if (stream == NULL || stream[0] == '\0') {
		HLT_WRN("Provided stream empty?", HLT_MJRWRN);
		return NULL;
	}

	int lin = 0, col = 0;
	struct stc_book *book = SyntacBookAllocate();
	book->type = type;

	char substrn[100]; substrn[0] = 0; //name or left consumption
	char substre[100];		   //elms or right production
	size_t nconsumeIdx = 0;
	int slen = strlen(stream);
	for (int i = 0; i <= slen; i++) {
		if (strncmp(stream+i, "->", 2) == 0) { //rule name found
			//HLT_WRNLC("Found Name!", lin, col, HLT_DEBUG);

			int sublen = i - nconsumeIdx;
			if (sublen <= 0) HLT_ERRLC("Empty Rule Name.", lin, col);
			if (sublen >= 100) HLT_ERRLC("Syntac does not support names longer than 99 characters!", lin, col);

			strncpy(substrn, stream+nconsumeIdx, sublen);
			substrn[sublen] = '\0';
			nconsumeIdx = i+2;
			
			if (warn_level == HLT_DEBUG) printf("Name found: '%s'\n", substrn);
		} else if (stream[i] == '\n' || stream[i] == '\0' || stream[i] == EOF) { //new line or eofS
			//HLT_WRNLC("Found newline/end.", lin, col, HLT_DEBUG);
			if (substrn[0] == '\0') {
				nconsumeIdx = i+1;
				substrn[0] = '\0';
				substre[0] = '\0';
				lin++; col = -1;
				continue; //no name, no rule
			}
			//HLT_WRNLC("Found Definition.", lin, col, HLT_DEBUG);

			int sublen = i - nconsumeIdx;
			if (stream[i] != '\n') sublen++; //include last char
			if (sublen <= 0) HLT_WRNLC("Empty Rule Definition.", lin, col, HLT_VERBSE);
			if (sublen >= 100) HLT_ERRLC("Syntac does not support definitions longer than 99 characters!", lin, col);

			if (sublen > 0) strncpy(substre, stream+nconsumeIdx, sublen);
			substre[sublen] = 0;

			SyntacBookRuleAdd(book, substrn, substre);
			if (warn_level == HLT_DEBUG) printf("Definition found: '%s'\n", substre);

			//Reset
			nconsumeIdx = i+1;
			substrn[0] = '\0';
			substre[0] = '\0';
			lin++; col = -1;
		}
		col++;
	}

	return book;
}

