#ifndef __HEPHAESTUS_SYNTACTICAL_ANALYZER__
#define __HEPHAESTUS_SYNTACTICAL_ANALYZER__
#include <stdbool.h>

//Public facing container of grammatical rules
typedef struct stc_book SyntacBook;

//Types of Grammar Trees
typedef enum stc_parsing_style {
	STC_LL0 = 0, 	//Top-Down
	STC_LL1, 	//Top-Down  w/ Lookahead
	STC_LR0, 	//Bottom-Up
	STC_LR1, 	//Bottom-Up w/ Lookahead
} SyntacTreeType;

typedef struct stc_tree_node {
	enum stc_parsing_style type;
	struct stc_tree_node *children; //null-terminated
	char *rule_name; //Grammar Rule Name

	int line_start, col_start;
	int line_end, col_end;
} SyntacTreeNode;

/*----------------------------Books----------------------------*/

//Allocate and Populate a rule book from a file
//Parse type must be on first line of file
SyntacBook * SyntacBookFromFile(char *file_name);

//Allocate and Populate a rule book from a string stream
SyntacBook * SyntacBookFromString(char *stream, SyntacTreeType type);

//Validate grammar book can be built into table
bool SyntacBookValidate(SyntacBook *);

//Allocate an empty rule book
SyntacBook * SyntacBookAllocate();

//Add a new definition to a rule book (dynamic allocation of left and right)
void SyntacBookRuleAdd(SyntacBook *book, char *left, char *right);

//Free a rule book
void SyntacBookFree(SyntacBook *);

/*----------------------------Trees----------------------------*/

//Generate a Syntactical Tree representation of tokens 
//Expects token stream to be null-terminated 
//Also assumes that it receives LexicTokens (void * -> LexicToken *)
SyntacTreeNode * SyntacTreeFromTokens(void *stream, SyntacBook *book);

//Generate a Syntactical Tree representation of string stream
//Expects token string stream to be null-terminated (stream[x] == NULL || stream[x][0] == 0)
SyntacTreeNode * SyntacTreeFromStream(char **stream, SyntacBook *book);

//Free a null-terminated Syntactical Tree
void SyntacTreeFree(SyntacTreeNode *);

#endif
