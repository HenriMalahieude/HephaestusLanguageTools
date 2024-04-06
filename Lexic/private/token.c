#include "../lexic.h"
#include "lexic_internal.h"
#include "warn.h"
#include "regex.h"

char * ftostr(char *file_name);

LexicToken * Lexic_Token_Stream_File(char *file_name, LexicVocabulary *vocab) {
	if (strlen(file_name) <= 0) Lexic_Error("Token Stream File. Was given empty string for file name!");

	char *strm = ftostr(file_name);

	struct lexic_token *tstrm = Lexic_Token_Stream_String(strm, vocab);

	free(strm);
	return tstrm;
}

LexicToken * Lexic_Token_Stream_String(char *stream, LexicVocabulary *vocab) {
	regex_line_no = 1;
	regex_colu_no = 1;

	size_t stlen = strlen(stream);
	char substring[100]; size_t nconsumed_ind = 0;
	int lst_rgx_match = -1; //negative signifies no match
	size_t lst_match_size = 0;
	for (size_t i = 0; i < stlen; i++) {
		int sublen = i - nconsumed_ind;
		if (sublen <= 0) continue; //don't support 0-string matches

		
	}
}

char ** Lexic_Token_Name_Stream_File(char *file_name, LexicVocabulary *vocab) {
	if (strlen(file_name) <= 0) Lexic_Error("Token Name Stream File. Was given empty string for file name!");

	char *strm = ftostr(file_name);

	char **namestrm = Lexic_Token_Name_Stream_String(strm, vocab);

	free(strm);
	return namestrm;
}

char ** Lexic_Token_Name_Stream_String(char *stream, LexicVocabulary *vocab) {
	if (strlen(stream) <= 0) return NULL;

	struct lexic_token *tstrm = Lexic_Token_Stream_String(stream, vocab);

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
