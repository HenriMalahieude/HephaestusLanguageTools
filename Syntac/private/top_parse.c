#include "syntac_internal.h"

//Null-Terminated Parsing Table
struct stc_top_table_entry * TopValidate(SyntacBook *) {
	
}

struct stc_tree_node * TopParseStream(char **stream, SyntacBook *book) {
	struct stc_top_table_entry *parsing_table = TopValidate(book);
	if (parsing_table == NULL) {
		HLT_WRN("Could not validate Syntax Book. Parsing table construction failed.", HLT_MJRWRN);
		return NULL;
	}
}

struct stc_tree_node * TopParseTokens(void *stream, SyntacBook *book) {
	struct stc_top_table_entry *parsing_table = TopValidate(book);
	if (parsing_table == NULL) {
		HLT_WRN("Could not validate Syntax Book. Parsing table construction failed.", HLT_MJRWRN);
		return NULL;
	}
}
