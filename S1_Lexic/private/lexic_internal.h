#ifndef __HEPH_INTERNAL_PRIVATE_LEXIC__
#define __HEPH_INTERNAL_PRIVATE_LEXIC__

extern int regex_line_no, regex_colu_no;

/*Definition of a Token*/
struct token_definition {
	char *name;
	char *regex;
};

/*Entire Vocabulary Rules*/
struct token_vocabulary {
	struct token_definition *definitions;
	int def_count;
};

#endif
