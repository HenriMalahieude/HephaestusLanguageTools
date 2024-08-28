#include <stdlib.h>
#include <string.h>

#include "../lexic.h"
#include "lexic_internal.h"
#include "../../helpers/log/warn.h"
#include "../../helpers/string/handy.h"
#include "regex.h"


char ** LexicTokenNamesFromFile(char *file_name, LexicVocabulary *vocab) {
	if (file_name == NULL || file_name[0] == '\0') HLTError("Name Stream File. Given empty file_name?", regex_line_no, regex_colu_no);
	if (vocab == NULL) HLTError("Name Stream File. Given NULL Vocab?", regex_line_no, regex_colu_no);

	char *strm = ftostr(file_name);

	char **namestrm = LexicTokenNamesFromString(strm, vocab);

	free(strm);
	return namestrm;
}

char ** LexicTokenNamesFromString(char *stream, LexicVocabulary *vocab) {
	if (stream == NULL || stream[0] == '\0') return NULL;
	if (vocab == NULL) HLTError("Name Stream String. Given NULL Vocab?", regex_line_no, regex_colu_no);

	struct lxc_token *tstrm = LexicTokensFromString(stream, vocab);
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
	
	LexicTokensFree(tstrm);
	return namestrm;
}

void LexicTokenNamesFree(char **nstrm) {
	if (nstrm == NULL) return;

	for (char **tptr = nstrm; *tptr != NULL; tptr++) {
		free(*tptr);
	}

	free(nstrm);
}
