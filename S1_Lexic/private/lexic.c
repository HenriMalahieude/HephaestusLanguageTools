#include <stdio.h>
#include "../lexic.h"
#include "regex.h"

int regex_line_no = 0;
int regex_colu_no = 0;

void Lexic_Error(char *msg) {
	printf("Lexic Error @ L%d, C%d: %s\n", regex_line_no, regex_colu_no, msg);
	exit(1);
}

void Lexic_Warn(char *msg) {
	printf("Lexic Warning @ L%d, C%d: %s\n", regex_line_no, regex_colu_no, msg);
}

//NOTE: Dynamically allocates a string. Expects it to be freed by caller
char* ftostr(char *file_name) {
	FILE *read_from = fopen(file_name, "r");
	if (read_from == NULL) Lexic_Error("Could not read from file. Does it exist?");

	long file_size = 0;
	fseek(read_from, 0, SEEK_END); //Move to eof
	file_size = ftell(read_from); //Amount of characters
	rewind(read_from); //Back to top
	
	char *strm = (char*)calloc(1, file_size+1); 
	if (strm == NULL) {
		fclose(read_from);
		Lexic_Error("Failed to allocate memory for file");
	}
	
	if (1 != fread(strm, file_size, 1, read_from)) {
		fclose(read_from);
		free(strm);
		Lexic_Error("Failed to read from supplied file");
	}

	fclose(read_from);
	return strm;
}

struct token_vocabulary Lexic_Vocabulary_Make_File(char *file_name) {
	char *strm = ftostr(file_name);
	
	struct token_vocabulary dictionary = Lexic_Vocabulary_Make_Stream(strm);

	free(strm);
	return dictionary;
}

//Expecting an input of "name: regex\n"+
struct token_vocabulary Lexic_Vocabulary_Make_Stream(char *stream) {
	regex_line_no = 1;
	regex_colu_no = 1;

	size_t consume_pos = 0;
	struct token_vocabulary vocab;
	vocab.definitions = NULL;
	vocab.def_count = 0;
	
	bool defining = false;
	char *lname = NULL;
	for (size_t pos = 0; pos < strlen(stream); pos++) {
		char cur = stream[pos];

		if (!defining && cur == '\n') {
			consume_pos = pos+1;
		} else if (!defining && cur == ':') { //Token Name
			lname = calloc(pos - consume_pos + 1, sizeof(char);
			strncpy(lname, stream+consume_pos, pos - consume_pos);
			//TODO: Eliminate Whitespace

		} else if (defining && cur == '\n') { //Token Definition
			
		}

		if (cur == '\n') {
			regex_line_no += 1;
			regex_colu_no = 1;
		} else {
			regex_colu_no += 1;
		}
	}

	if (defining) Lexic_Error("Cannot have an empty token definition ??");

	return vocab;
}

struct token_instance* Lexic_Token_Stream_File(char *file_name, struct token_vocabulary *vocab) {
	char *strm = ftostr(file_name);

	struct token_instance *tstrm = Lexic_Token_Stream_String(strm, vocab);

	free(strm);
	return tstrm;
}

struct token_instance* Lexic_Token_Stream_String(char *stream, struct token_vocabulary *vocab) {

}

char ** Lexic_Token_Name_Stream_File(char *file_name, struct token_vocabulary *vocab) {
	char *strm = ftostr(file_name);

	char **namestrm = Lexic_Token_Name_Stream_String(strm, vocab);

	free(strm);
	return namestrm;
}

char ** Lexic_Token_Name_Stream_String(char *stream, struct token_vocabulary *vocab) {
	struct token_instance *tstrm = Lexic_Token_Stream_String(stream, vocab);

	char **namestrm = NULL;
	int name_strm_cnt = 0;

	for (;tstrm->name != NULL; tstrm++) {
		char *name = tstrm->name;
		
		name_strm_cnt += 1;
		namestrm = realloc(namestrm, name_strm_cnt * sizeof(char*));
		namestrm[name_strm_cnt-1] = (char*)calloc(strlen(name)+1, sizeof(char));
	}

	namestrm = realloc(namestrm, (name_strm_cnt+1) * sizeof(char*));
	namestrm[name_strm_cnt] = NULL; //Properly delineate the array

	return namestrm;
}
