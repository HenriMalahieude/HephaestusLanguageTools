#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../lexic.h"
#include "lexic_internal.h"
#include "regex.h"
#include "warn.h"

//allocates a new string which is expected to be freed by caller
//returns if was able to make a non-empty string
bool trim(char *input, char **nstr) {
	int front = 0;
	
	for (front = 0; front < strlen(input); front++) {
		if (!isspace(input[front])) break;
	}

	if (front >= strlen(input)) return false;

	int back;
	for (back = front; back < strlen(input); back++) {
		if (isspace(input[back+1]) break;
	}
	
	int len = (back - front) + 1;
	if (len <= 0) return false;

	*nstr = calloc(len+1, sizeof(char));
	strncpy(*nstr, input+front, len);
}

void add_def(struct token_vocabulary *dict, struct token_definition def) {
	dict->def_count++;
	realloc(dict->definitions, dict->def_count * sizeof(struct token_definition));
	dict->definitions[dict->def_count-1] = def;
}

//Expecting an input of "name: regex\n"+
struct token_vocabulary Lexic_Vocabulary_Make_Stream(char *stream) {
	regex_line_no = 1;
	regex_colu_no = 1;
	
	struct token_vocabulary dictionary = {.definitions = NULL, .def_count = 0};

	char substr[100]; size_t nconsume_ind = 0;
	char *nme = NULL;
	int slen = strlen(stream);
	for (int i = 0; i < slen; i++) {
		if (stream[i] == ':') { //acquire the name
			Lexic_Warn("Lexic_Vocabulary_Make_Stream. Found Name.", LWT_DEBUG);

			int sublen = (i-1) - nconsumed_ind + 1;
			if (sublen <= 0) Lexic_Error("Empty Token Name.");
			if (sublen >= 100) Lexic_Error("Lexic does not support names longer than 99 characters!");

			strncpy(substr, stream+nconsume_ind, sublen);
			substr[sublen] = '\0';
			if (!trim(substr, &nme)) Lexic_Error("Empty Token Name.");
			nconsumed_ind = i+1;
		} else if (stream[i] == '\n') {
			Lexic_Warn("Lexic_Vocabulary_Make_Stream. Found Definition.", LWT_DEBUG);

			if (nme == NULL) Lexic_Error("Missing Token Name for Definition.");

			int sublen = (i-1) - nconsumed_ind + 1;
			if (sublen <= 0) Lexic_Error("Empty Token Definition.");
			if (sublen >= 100) Lexic_Error("Lexic does not support definitions longer than 99 characters!");
			strncpy(substr, stream+nconsume_ind, sublen);
			substr[sublen] = '\0';
			struct token_definition dd = {.name = nme, .regex = NULL};
			if (!trim(substr, &dd.regex)) Lexic_Error("Empty Token Definition.");
			if (!Regex_Validate(dd.regex)) Lexic_Error("Could not create Token Dictionary.");
			add_def(&dictionary, dd);
			
			nme = NULL;
			nconsumed_ind = i+1;
			regex_line_no++; regex_colu_no = 0;
		}
		regex_colu_no++;
	}

	return dictionary;
}
