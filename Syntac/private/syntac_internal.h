#include "../syntac.h"

#ifndef __HEPH_INTERNAL_PRIVATE_SYNTAC__
#define __HEPH_INTERNAL_PRIVATE_SYNTAC__

#define RIGHT_DELIM ':'

//Rule Definition
// left -> right
// a -> b c
struct stc_rule {
	char *name; //also the left part of the rule
	char **elements; //also the right part of the rule (null-terminated)
};

//Rule Book
struct stc_book {
	enum stc_parsing_style type;
	struct stc_rule *rules;
	int rule_count;
};

SyntacTreeNode * LL0ParseTokens(void *stream, SyntacBook *book);
SyntacTreeNode * LL1ParseTokens(void *stream, SyntacBook *book);
SyntacTreeNode * LR0ParseTokens(void *stream, SyntacBook *book);
SyntacTreeNode * LR1ParseTokens(void *stream, SyntacBook *book);

SyntacTreeNode * LL0ParseStream(char **stream, SyntacBook *book);
SyntacTreeNode * LL1ParseStream(char **stream, SyntacBook *book);
SyntacTreeNode * LR0ParseStream(char **stream, SyntacBook *book);
SyntacTreeNode * LR1ParseStream(char **stream, SyntacBook *book);

bool LL0Validate(SyntacBook *);
bool LL1Validate(SyntacBook *);
bool LR0Validate(SyntacBook *);
bool LR1Validate(SyntacBook *);
#endif
