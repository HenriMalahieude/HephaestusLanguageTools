#include <stdlib.h>
#include <string.h>

#include "syntac_private.h"
#include "../../helpers/string/handy.h"
#include "../../helpers/log/warn.h"

//Allocate an Empty Rule Book
SyntacBook * SyntacBookAllocate() {
	SyntacBook *book = malloc(sizeof(struct stc_book));

	if (book == NULL) {HLT_AERR("Attempted malloc of a book failed?");}
	
	book->rules = NULL;
	book->rule_count = 0;

	return book;
}

//Free a rule book (assumes everything within the rulebook and rules is dynamically allocated)
void SyntacBookFree(SyntacBook *book) {
	if (book == NULL) {
		HLT_UWRN("Supplied book pointer was null?", HLT_STDWRN);
		return;
	}

	for (int i = 0; i< book->rule_count; i++){
		struct stc_rule *rule = &book->rules[i];
		if (rule == NULL) {
			HLT_UWRN("Rule pointer was null?", HLT_MJRWRN);
			continue;
		}

		free(rule->name); rule->name = NULL;
		for (int j = 0; rule->elements[j] != NULL; j++) {
			free(rule->elements[j]); //free all elements inside
		}
		free(rule->elements); rule->elements = NULL; //free container
	}

	if (book->rules != NULL) free(book->rules); 
	book->rules = NULL; book->rule_count = 0;
	free(book);
}	

void SyntacBookRuleAdd(SyntacBook *book, char *left, char *right) {
	if (book == NULL) {
		HLT_UWRN("Supplied book pointer was null?", HLT_STDWRN);
		return;
	}

	if (left == NULL || left[0] == '\0') {
		HLT_UWRN("Supplied 'left' of rule was null/empty?", HLT_STDWRN);
		return;
	}

	if (right == NULL || left[0] == '\0') {
		HLT_UWRN("Supplied 'right' of rule was null/empty?", HLT_STDWRN);
		return;
	}

	//Initialize the rule + name
	struct stc_rule *rule = malloc(sizeof(struct stc_rule));
	rule->name = calloc(strlen(left)+1, sizeof(char)); 
	trim(left, &rule->name);

	//Count Amount of Elements (delimited by space)
	int elm_cnt = 0; bool lstwasSpace = true;
	for (int i = 0; i < strlen(right); i++) {
		if (isspace(right[i])) {
			if (!lstwasSpace) elm_cnt++;
			lstwasSpace = true;
		} else {
			lstwasSpace = false;
		}
	}

	if (elm_cnt <= 0) {HLT_UERR("Element count of 'right' was <= zero?");}
	
	//Allocate amount of elements detected
	rule->elements = malloc(sizeof(char *) * (elm_cnt+1));
	rule->elements[elm_cnt] = NULL; //NULL-terminate

	//Parse the elements now
	size_t elm_i = 0;
	size_t nconsumeIdx = 0;
	char substr[100];
	for (int i = 1; i < strlen(right); i++) {
		if ((isspace(right[i]) || i == strlen(right)-1) && nconsumeIdx != i) {
			int len = (i - nconsumeIdx);
			strncpy(substr, right + nconsumeIdx, len);
			substr[len] = 0;
			trim(substr, &rule->elements[elm_i++]); //calloc a trimmed version of substr
		}
	}

	if (elm_i < elm_cnt) {
		snprintf(substr, 100, "Element parsing recorded %d, but expected %d?", elm_i, elm_cnt);
		HLT_UERR(substr);
	}

	//Place into the book
	book->rules = realloc(book->rules, sizeof(struct stc_rule) * ++(book->rule_count)); //unless you have a better idea...
	book->rules[book->rule_count-1] = rule;
}

SyntacBook * SyntacBookFromFile(char *file_name){
	if (file_name == NULL || file_name[0] == '\0') {
		HLTWarn("SyntacBookFromFile. Supplied file_name was null/empty?");
		return NULL;
	}

	char *cntnts = ftostr(file_name);
	if (cntnts == NULL) {HLT_AERR("Could not convert file to string?");}

	int nl; //new line location
	for (nl = 0; cntnts[nl] == '\0' || cntnts[nl] == '\n'; nl++);

	char substr[100];
	strncpy(substr, cntnts, nl);

	enum stc_parsing_style type;
	if (strcmp(substr, "LL0") == 0) 	type = STC_LL0;
	else if (strcmp(substr, "LL1") == 0) 	type = STC_LL1;
	else if (strcmp(substr, "LR0") == 0)	type = STC_LR0;
	else if (strcmp(substr, "LR1") == 0) 	type = STC_LR1;
	else {HLT_UERR("File did not contain proper parsing type?");}
		
	SyntacBook *book = SyntacBookFromString(cntnts + nl + 1, type); //past the new line
	
	free(cntnts);
	return book;
}

SyntacBook * SyntacBookFromString(char *stream, SyntacTreeType type) {
	int lin = 0, col = 0;

	struct stc_book *book = SyntacBookAllocate();
	book->type = type;

	char substrn[100]; substrn[0] = 0;
	char substre[100];
	size_t nconsumeIdx = 0;
	int slen = strlen(stream);
	for (int i = 0; i < slen; i++) {
		if (strncmp(stream+i, "->", 2) == 0) { //rule name found
			HLTWarn("Book Make String. Found Name.", lin, col, HLT_DEBUG);

			int sublen = (i-1) - nconsumeIdx + 1;
			if (sublen <= 0) HLTError("Empty Rule Name.", lin, col);
			if (sublen >= 100) HLTError("Syntac does not support names longer than 99 characters!", lin, col);

			strncpy(substrn, stream+nconsumeIdx, sublen);
			substrn[sublen] = '\0';
			
			if (warn_level == HLT_DEBUG) printf("name found: %s\n", substrn);
		} else if (stream[i] == '\n' || i >= slen-1) { //new line or eof
			HLTWarn("Book Make String. Found Definition.", lin, col, HLT_DEBUG);

			if (substrn[0] == 0) continue;

			int sublen = (i-1) - nconsumeIdx + 1;
			if (i >= slen-1) sublen++;
			if (sublen <= 0) HLTError("Empty Rule Definition.", lin, col);
			if (sublen >= 100) HLTError("Syntac does not support definitions longer than 99 characters!", lin, col);

			strncpy(substre, stream+nconsumeIdx, sublen);
			substre[sublen] = 0;

			SyntacBookRuleAdd(book, substrn, substre);
			substrn[0] = '\0';
			substre[0] = '\0';
			lin++; col = 0;
		}
		col++;
	}

	return book;
}

