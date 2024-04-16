#include <stdlib.h>
#include <string.h>

#include "../lexic.h"
#include "lexic_internal.h"
#include "warn.h"
#include "regex.h"

char * ftostr(char *file_name);

char ** Lexic_Token_Name_Stream_Make_From_File(char *file_name, LexicVocabulary *vocab) {
	if (file_name == NULL || file_name[0] == '\0') Lexic_Error("Name Stream File. Given empty file_name?");
	if (vocab == NULL) Lexic_Error("Name Stream File. Given NULL Vocab?");

	char *strm = ftostr(file_name);

	char **namestrm = Lexic_Token_Name_Stream_Make_From_String(strm, vocab);

	free(strm);
	return namestrm;
}

char ** Lexic_Token_Name_Stream_Make_From_String(char *stream, LexicVocabulary *vocab) {
	if (stream == NULL || stream[0] == '\0') return NULL;
	if (vocab == NULL) Lexic_Error("Name Stream String. Given NULL Vocab?");

	struct lexic_token *tstrm = Lexic_Token_Stream_Make_From_String(stream, vocab);
	if (tstrm == NULL || tstrm[0].definition_name == NULL || tstrm[0].definition_name[0] == '\0') return NULL;

	char **namestrm = NULL;
	int name_strm_cnt = 0;

	for (size_t t_ind = 0; tstrm[t_ind].definition_name != NULL; t_ind++) {
		char *name = tstrm[t_ind].definition_name;
		
		name_strm_cnt += 1;
		namestrm = realloc(namestrm, name_strm_cnt * sizeof(char*));
		namestrm[name_strm_cnt-1] = (char*)calloc(strlen(name)+1, sizeof(char));
		strcpy(namestrm[name_strm_cnt-1], name);
	}

	namestrm = realloc(namestrm, (name_strm_cnt+1) * sizeof(char*));
	namestrm[name_strm_cnt] = NULL; //Properly delineate the array
	
	Lexic_Token_Stream_Free(tstrm);
	return namestrm;
}

void Lexic_Token_Name_Stream_Free(char **nstrm) {
	if (nstrm == NULL) return;

	for (char **tptr = nstrm; *tptr != NULL; tptr++) {
		free(*tptr);
	}

	free(nstrm);
}
