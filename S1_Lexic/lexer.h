#ifndef __HEPHAESTUS_LEXICAL_ANALYZER__
#define __HEPHAESTUS_LEXICAL_ANALYZER__

struct regex;
extern int regex_line_no;
extern int regex_colu_no;

/*Definition of a Token*/
struct token_definition {
	char *name;
	char *raw_regex;

	//Array/Sequence of matches before this token is matched
	struct regex *definition; //NOTE: This is NULL terminated
};

/*Entire Vocabulary Rules*/
struct token_vocabulary {
	struct TokenDefinition *definitions;
	int definition_count;
};

/*An Instance of a Token (Definition)*/
struct token_instance {
	char *definition_name;
	char *matching_input;
	int line;
	int col;
};

/*Make Lexical Vocabulary From a File*/
struct token_vocabulary Lexic_Vocabulary_Make_File(char *file_name);

/*Make Lexical Vocabulary From a String*/
struct token_vocabulary Lexic_Vocabulary_Make_Stream(char *stream);

/*Read File and Create a NULL-Terminated Token Stream*/
struct token_instance* Lexic_Token_Stream(char *file_name, struct token_vocabulary vocab);

#endif
