#include "syntac_private.h"
#include "../../helpers/log/warn.h"
#include "../../helpers/string/handy.h"

SyntacTreeNode * SyntacTreeFromTokens(LexicToken *stream, Syntac *book) {
	//TODO
}

SyntacTreeNode * SyntacTreeFromStream(char **stream, SyntacBook *book) {
	//TODO
}

void SyntacTreeFree(SyntacTreeNode *tree) {
	if (tree == NULL) {
		HLTWarn("SyntacTreeFree. Supplied tree pointer was null?", 0, 0, HLT_STDWRN);
		return;
	}

	if (tree->children[0] != NULL) {
		for (int i = 0; tree->children[i] != NULL; i++) {
			SyntacTreeFree(tree->children[i]);
		}
	}

	free(tree->rule_name); tree->rule_name = NULL;
	free(tree->contains); tree->contains = NULL;

	free(tree);
}
