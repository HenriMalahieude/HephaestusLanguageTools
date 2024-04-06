#include "../lexic.h"
#include "lexic_internal.h"
#include "warn.h"
#include "regex.h"

char * ftostr(char *file_name);

LexicToken * Lexic_Token_Stream_Make_From_File(char *file_name, LexicVocabulary *vocab) {
	if (file_name == NULL || file_name[0] == '\0') Lexic_Error("Token Stream File. Given empty file name?");
	if (vocab == NULL) Lexic_Error("Token Stream File. Given NULL Vocab?");

	char *strm = ftostr(file_name);

	struct lexic_token *tstrm = Lexic_Token_Stream_String(strm, vocab);

	free(strm);
	return tstrm;
}

LexicToken * Lexic_Token_Stream_Make_From_String(char *stream, LexicVocabulary *vocab) {
	if (vocab == NULL) Lexic_Error("Token Stream String. Given NULL Vocab?");

	regex_line_no = 1;
	regex_colu_no = 1;

	struct lexic_token *tstrm = malloc(sizeof(struct lexic_token)); size_t t_ind = 0; //unused index	

	if (stream == NULL || stream[0] == '\0') {
		tstrm[0].name = NULL;
		tstrm[0].regex = NULL;
		return tstrm;
	}

	size_t stlen = strlen(stream);
	char substring[100]; size_t nconsumed_ind = 0;
	int lst_rgx_match = -1; //negative signifies no match
	size_t lst_match_size = 0;

	for (size_t i = 0; i < stlen; i++) {
		int sublen = i - nconsumed_ind;
		if (sublen <= 0) continue; //don't support 0-string matches

		
	}
}

//assuming that definition_name and matching_input fields are malloc copies
void Lexic_Token_Stream_Free(LexicToken *tstrm) {
	if (tstrm == NULL) return;

	struct lexic_token current = tstrm[0];
	size_t cur_ind = 0;
	while(current.definition_name != NULL) {
		free(current.definition_name);
		free(current.matching_input);
		current = tstrm[++cur_ind];
	}
	
	free(tstrm);
}
