#include "../lexic.h"
#include "lexic_internal.h"
#include "warn.h"
#include "regex.h"

char * ftostr(char *file_name);

//returns the index of the matching token definition to input
int match_within_vocabulary(struct lexic_vocabulary *vocab, char *input) {	
	int match_index = -1;
	
	for (int defi = 0; defi < vocab->def_count; defi++) {
		struct token_definition tdef = vocab->definitions[defi];
		if (Regex_Match(input, tdef.regex)) {
			match_index = defi;
			break;
		}
	}

	return match_index;
}

void add_token_to_tstream(struct lexic_token **tstrm, size_t *tlen, struct token_definition tdef, char *match) {
	*tstrm = realloc(*tstrm, (++*tlen) * sizeof(struct lexic_token));
	if (*tstrm) Lexic_Error("_add_token_to_tstream. reallocation failed?");

	tstrm[*tlen-1].definition_name = NULL; //delineate
	
	if (*tlen > 1) {
		struct lexic_token *tok = tstrm[tlen-2];
		tok->definition_name = calloc(strlen(tdef.name)+1, sizeof(char)); strcpy(tok->definition_name, tdef.name);
		tok->matching_input = calloc(strlen(match)+1, sizeof(char)); strcpy(tok->matching_input, match);
		tok->line = regex_line_no;
		tok->col = regex_colu_no;
	}
}

LexicToken * Lexic_Token_Stream_Make_From_File(char *file_name, LexicVocabulary *vocab) {
	if (file_name == NULL || file_name[0] == '\0') Lexic_Error("Token Stream File. Given empty file name?");
	if (vocab == NULL) Lexic_Error("Token Stream File. Given NULL Vocab?");

	char *strm = ftostr(file_name);

	struct lexic_token *tstrm = Lexic_Token_Stream_String(strm, vocab);

	free(strm);
	return tstrm;
}

//Random TODO Idea: What if we made a regex tree which could reduce this O(m*n) procedure down, would it improve time complexity?
LexicToken * Lexic_Token_Stream_Make_From_String(char *stream, LexicVocabulary *vocab) {
	if (vocab == NULL) Lexic_Error("Token Stream String. Given NULL Vocab?");
	if (stream == NULL) Lexic_Error("Token Stream String. Given NULL stream?");
	if (stream[0] == '\0') return NULL;

	regex_line_no = 1;
	regex_colu_no = 1;

	struct lexic_token *tstrm = malloc(sizeof(struct lexic_token)); tstrm->definition_name = NULL;
	size_t tlen = 1; //includes "null" termination

	size_t stlen = strlen(stream);
	char substring[100];
	size_t nconsumed_ind = 0; //inclusive not consumed idx

	int lst_match = -1; //negative signifies no match
	int lst_start = -1;

	for (size_t i = 0; i < stlen; i++) {
		int sublen = i - nconsumed_ind; //this means i is exclusive
		if (sublen <= 0) continue; //don't support 0-string matches
		if (sublen >= 99) {
			Lexic_Warn("Token Stream String. Reached 99 token limit before consumption.", LWT_MJRWRN);
			break;
		}

		strncpy(substring, stream+nconsumed_ind, sublen);
		substring[sublen] = '\0';

		int cur_match = match_within_vocabulary(vocab, substring);
		int cur_start = nconsumed_ind;

		if (cur_match < 0 && sublen > 1) { //need to drop characters at front?
			for (int j = 1; j < sublen-1; j++) { //1st idx -> 2nd last idx, so it doesn't drop last character
				char *subsub = substring + j;
				cur_match = match_within_vocabulary(vocab, subsub);
				if (cur_match > -1) {
					cur_start = nconsumed_ind+j;
					break;
				}
			}
		}

		if (cur_match >= 0) {
			if (i < stlen-1) {
				lst_match = cur_match;
				lst_start = cur_start;
			} else { //eof, consume regardless
				if (cur_start != nconsumed_ind) Lexic_Warn("Dropping unmatched characters at front of unconsumed character sequence! (EOF)", LWT_VERBSE);
				add_token_to_tstream(&tstrm, &tlen, vocab->definitions[cur_match], substring+(cur_start-nconsumed_ind));
			}
		} else if (lst_match >= 0) {
			if (lst_start != nconsumed_ind) Lexic_Warn("Dropping unmatched characters at front of unconsumed character sequence!", LWT_VERBSE);
			add_token_to_tstream(&tstrm, &tlen, vocab->definitions[cur_match], substring+(cur_start-nconsumed_ind));
		}

		if (stream[i] == '\n') {
			regex_line_no++;
			regex_colu_no = 1;
		} else {
			regex_colu_no++;
		}
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
