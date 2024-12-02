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

struct stc_top_table_entry {
	char *token_seen;
	struct stc_rule *rule_to_use;
};

struct stc_bot_table_entry {};

//Outputs: array = {{rule.name, set of first/follow tokens}, ...};
char **firsts(SyntacBook *book);
char **follows(SyntacBook *book);

struct stc_tree_node * TopParseTokens(void *stream, SyntacBook *book);
struct stc_tree_node * BotParseTokens(void *stream, SyntacBook *book);

struct stc_tree_node * TopParseStream(char **stream, SyntacBook *book);
struct stc_tree_node * BotParseStream(char **stream, SyntacBook *book);

//struct stc_top_table_entry * TopValidate(SyntacBook *);
//struct stc_bot_table_entry * BotValidate(SyntacBook *);
#endif
