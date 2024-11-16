#include "syntac_private.h"
#include "../../helpers/log/warn.h"
#include "../../helpers/string/handy.h"

SyntacTreeNode * SyntacTreeFromTokens(void *stream, Syntac *book) {
	if (stream == NULL) {
		HLT_WRN("Token stream provided is empty?", HLT_MJRWRN);
		return NULL;
	}

	if (book == NULL) {
		HLT_WRN("Rule Book given null?", HLT_MJRWRN);
		return NULL;
	}

	if (!SyntacBookValidate(book)) {
		HLT_WRN("Rule Book given cannot be formed into a parsing table!", HLT_MJRWRN);
		return NULL;
	}

	SyntacTreeType typ = book->type;
	switch (typ) {
		case (STC_LL0):
			return LL0ParseTokens(stream, book);
		case (STC_LL1):
			return LL1ParseTokens(stream, book);
		case (STC_LR0):
			return LR0ParseTokens(stream, book);
		case (STC_LR1):
			return LR1ParseTokens(stream, book);
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

	if (!SyntacBookValidate(book)) {
		HLT_WRN("Rule Book given cannot be formed into a parsing table!", HLT_MJRWRN);
		return NULL;
	}

	SyntacTreeType typ = book->type;
	switch (typ) {
		case (STC_LL0):
			return LL0ParseStream(stream, book);
		case (STC_LL1):
			return LL1ParseStream(stream, book);
		case (STC_LR0):
			return LR0ParseStream(stream, book);
		case (STC_LR1):
			return LR1ParseStream(stream, book);
		default:
			HLT_AERR("Unknown Parsing Type!?!?");
			break;
	}
	
	HLT_AERR("Unreachable!?!?");
	return NULL;
}

bool SyntacBookValidate(SyntacBook *book) {
	if (book == NULL) {
		HLT_WRN("Rule book null?", HLT_MJRWRN);
		return false;
	}

	SyntacTreeType typ = book->type;
	switch (typ) {
		case (STC_LL0):
			return LL0Validate(book);
		case (STC_LL1):
			return LL1Validate(book);
		case (STC_LR0):
			return LR0Validate(book);
		case (STC_LR1):
			return LR1Validate(book);
		default:
			HLT_AERR("Unknown Parsing Type!?!?");
			break;
	}
	
	HLT_AERR("Unreachable!?!?");
	return false;
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
