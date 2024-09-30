#include "../syntac.h"

#ifndef __HEPH_INTERNAL_PRIVATE_SYNTAC__
#define __HEPH_INTERNAL_PRIVATE_SYNTAC__

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

#endif
