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

char* ftostr(char *file_name) {
	FILE *read_from = fopen(file_name, "r");
	if (read_from == NULL) Lexic_Error("Could not read from file. Does it exist?");

	long file_size = 0;
	fseek(read_from, 0, SEEK_END); //Move to eof
	file_size = ftell(read_from); //Amount of characters
	rewind(read_from); //Back to top
	
	char *strm = (char*)calloc(1, file_size+1); //file_size = ftell; which returns bytes
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

	fclose(read_from);
	return strm;
}

struct token_vocabulary* Lexic_Vocabulary_Make_File(char *file_name) {
	char *strm = ftostr(file_name);
	
	struct token_vocabulary *dictionary = Lexic_Vocabulary_Make_Stream(strm);

	free(strm);
	return dictionary;
}

//Expecting an input of "name: regex\n"+
struct token_vocabulary* Lexic_Vocabulary_Make_Stream(char *stream) {
	regex_line_no = 1;
	regex_colu_no = 1;

	size_t consume_pos = 0;
	struct token_vocabulary *vocab = malloc(sizeof(struct token_vocabulary));
	vocab->definitions = NULL;
	vocab->def_count = 0;
	
	bool defining = false;
	char *lname = NULL;
	for (size_t pos = 0; pos < strlen(stream); pos++) {
		if (!defining) { //acquiring the name
			if (stream[pos] == ':') { //end of name
				if (consume_pos == pos) Lexic_Error("Cannot have an empty token name.");
				lname = calloc((pos-consume_pos)+1, sizeof(char));
				strncopy(lname, stream+consume_pos, pos-consume_pos);

				printf("Lexic Debug: token def name of %s\n", lname);

				consume_pos = pos+1;//consume up to name and colon
				defining = true;
			}
		} else { // defining the token's regex
			if (pos == (strlen(stream)-1) || stream[pos] == '\n') { //either eof, or new line
				if (consume_pos == pos) Lexic_Error("Cannot have an empty token definition.");
				struct token_definition *def = malloc(sizeof(struct token_definition));
				def->name = lname;
				
				int total = pos - consume_pos;
				if (stream[pos] == '\n') total -= 1; //Don't include the newline. Let's not waste time

				def->raw_regex = calloc(total+1, sizeof(char));
				strncopy(def->raw_regex, stream+consume_pos, total);

				printf("Lexic Debug: token def regex of %s\n", def->raw_regex);
				
				def->definition = Regex_New_Definition(def->raw_regex, &def->reg_count);
				
				vocab->def_count += 1;
				vocab->definitions = realloc(vocab->definitions, (vocab->def_count)*sizeof(struct token_definition));
				vocab->definitions[vocab->def_count-1] = def;
				
				consume_pos = pos+1;
				defining = false;
				lname = NULL;
			}
		}
	}

	if (defining) Lexic_Error("Last Token Definition not defined?");

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
