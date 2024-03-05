#include <stdio.h>
#include "../lexic.h"
#include "regex.h"

int regex_line_no = 0;
int regex_colu_no = 0;

void Lexic_Error(char *msg) {
	printf("Lexic Error @ L%d, C%d: %s\n", regex_line_no, regex_colu_no, msg);
	exit(1);
}

struct token_vocabulary* Lexic_Vocabulary_Make_File(char *file_name) {
	FILE *read_from = fopen(file_name, "r");
	if (read_from == NULL) Lexic_Error("Could not read from file. Does it exist?");

	long file_size = 0;
	fseek(read_from, 0, SEEK_END); //Move to eof
	file_size = ftell(read_from); //Amount of characters
	rewind(read_from); //Back to top
	
	char *strm = (char*)malloc(file_size+1); //file_size = ftell; which returns bytes
						 //if char ever changes from bytes to something else, this is screwed lol
	if (strm == NULL) {
		fclose(read_from);
		Lexic_Error("Failed to allocate memory for file");
	}
	
	if (1 != fread(strm, file_size, 1, read_from)) {
		fclose(read_from);
		free(strm);
		Lexic_Error("Failed to read from supplied file");
	}
	strm[file_size] = '\0';
	
	struct token_vocabulary *dictionary = Lexic_Vocabulary_Make_Stream(strm);

	fclose(read_from);
	free(strm);
	return dictionary;
}

struct token_vocabulary* Lexic_Vocabulary_Make_Stream(char *stream) {

}

struct token_instance* Lexic_Token_Stream_File(char *file_name, struct token_vocabulary *vocab) {

}

struct token_instance* Lexic_Token_Stream_String(char *stream, struct token_vocabulary *vocab) {

}

char ** Lexic_Token_Name_Stream_File(char *file_name, struct token_vocabulary *vocab) {

}

char ** Lexic_Token_Name_Stream_String(char *stream, struct token_vocabulary *vocab) {

}