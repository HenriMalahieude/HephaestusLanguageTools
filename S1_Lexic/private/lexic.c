#include <stdio.h>
#include "../lexic.h"
#include "lexic_internal.h"
#include "warn.h"
#include "regex.h"

int regex_line_no = 0;
int regex_colu_no = 0;

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

struct token_instance* Lexic_Token_Stream_File(char *file_name, struct token_vocabulary *vocab) {
	char *strm = ftostr(file_name);

	struct token_instance *tstrm = Lexic_Token_Stream_String(strm, vocab);

	free(strm);
	return tstrm;
}

struct token_instance* Lexic_Token_Stream_String(char *stream, struct token_vocabulary *vocab) {
	//TODO
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

	for (tstrm = tstrm;tstrm->name != NULL; tstrm++) {
		char *name = tstrm->name;
		
		name_strm_cnt += 1;
		namestrm = realloc(namestrm, name_strm_cnt * sizeof(char*));
		namestrm[name_strm_cnt-1] = (char*)calloc(strlen(name)+1, sizeof(char));
	}

	namestrm = realloc(namestrm, (name_strm_cnt+1) * sizeof(char*));
	namestrm[name_strm_cnt] = NULL; //Properly delineate the array

	return namestrm;
}
