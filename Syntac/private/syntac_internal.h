#include "../syntac.h"
#include "sets.h"

#ifndef __HEPH_INTERNAL_PRIVATE_SYNTAC__
#define __HEPH_INTERNAL_PRIVATE_SYNTAC__

// separator for the right production of rules
#define RIGHT_DELIM ':'

//Used for firsts and follows (empty)
//Should be impossible to get this through as a "name" or "elements[i]" of stc_rule
#define EPSILON "\1" 

//Limitations
#define FIRSTS_CHG_LIM 1000
#define SUBSTR_SZ 200

//Rule Definition
// left -> right 
// a -> b : c:xx :
// which is [ 'a' consumes 'b' 'c' 'xx' ('bcxx') ]
struct stc_rule {
	char *name; //also the left part of the rule
	char **elements; //also the right part of the rule (null-terminated)
					 
	char **first_set; // NULL-Terminated
	char **follow_set;
};

//Parsing Tables
//	TODO: Serializability?
struct stc_top_table_entry {
	char *token_seen;
	struct stc_rule *rule_to_use;
};

struct stc_bot_table_entry {
	char **tokens_seen;
};

//Rule Book
struct stc_book {
	enum stc_parsing_style type;
	struct stc_rule *rules;
	int rule_count;

	struct stc_top_table_entry *top_table;
	struct stc_bot_table_entry *bot_table;
};

bool is_terminal(struct stc_book *book, char *element);

void firsts_of_book(struct stc_book *book);
void follow_of_book(struct stc_book *book);

struct stc_tree_node * TopParseTokens(struct stc_book *book, void *stream);
struct stc_tree_node * TopParseStream(struct stc_book *book, char **stream);

struct stc_tree_node * BotParseTokens(struct stc_book *book, void *stream);
struct stc_tree_node * BotParseStream(struct stc_book *book, char **stream);
#endif
