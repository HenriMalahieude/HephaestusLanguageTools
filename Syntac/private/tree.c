#include "syntac_private.h"
#include "../../helpers/log/warn.h"
#include "../../helpers/string/handy.h"

SyntacTreeNode * SyntacTreeFromTokens(void *stream, Syntac *book) {
	if (stream == NULL) {
		HLT_WRN("Token stream provided is empty?", HLT_MJRWRN);
		return NULL;
	}

	if (book == NULL) {
		HLT_WRN("Rule Book null?", HLT_MJRWRN);
		return NULL;
	}

	SyntacTreeType typ = book->type;
	switch (typ) {
		case (STC_TOP):
			return TopParseTokens(stream, book);
		case (STC_BOT):
			return BotParseTokens(stream, book);
		default:
			HLT_AERR("Unknown Parsing Type!?!?");
			break;
	}
	
	HLT_AERR("Unreachable!?!?");
	return NULL;
}

SyntacTreeNode * SyntacTreeFromStream(char **stream, SyntacBook *book) {
	if (stream == NULL || *stream == NULL) {
		HLT_WRN("String stream provided is empty?", HLT_MJRWRN);
		return NULL;
	}

	if (book == NULL) {
		HLT_WRN("Rule Book null?", HLT_MJRWRN);
		return NULL;
	}

	SyntacTreeType typ = book->type;
	switch (typ) {
		case (STC_TOP):
			return TopParseStream(stream, book);
		case (STC_BOT):
			return BotParseStream(stream, book);
		default:
			HLT_AERR("Unknown Parsing Type!?!?");
			break;
	}
	
	HLT_AERR("Unreachable!?!?");
	return NULL;
}

void SyntacTreeFree(SyntacTreeNode *tree) {
	if (tree == NULL) {
		HLT_WRN("Supplied tree pointer was null?", HLT_MJRWRN);
		return;
	}

	if (tree->children[0] != NULL) {
		for (int i = 0; tree->children[i] != NULL; i++) {
			SyntacTreeFree(tree->children[i]);
		}
	}

	free(tree->rule_name); tree->rule_name = NULL;
	free(tree);
}
