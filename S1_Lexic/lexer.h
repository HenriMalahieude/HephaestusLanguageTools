#ifndef __HEPHAESTUS_LEXICAL_ANALYZER__
#define __HEPHAESTUS_LEXICAL_ANALYZER__

struct regex;
extern int regex_line_no;
extern int regex_colu_no;

/*Definition of a Token*/
struct token_definition {
	char *name;
	char *raw_regex;

	//Array/Sequence of matches before this token is fully matched
	struct regex **definition; 
	int reg_count;
};

/*Entire Vocabulary Rules*/
struct token_vocabulary {
	struct TokenDefinition *definitions;
	int def_count;
};

/*An Instance of a Token (Definition)*/
struct token_instance {
	char *definition_name;
	char *matching_input;
	int line;
	int col;
};

/*Make Lexical Vocabulary*/
struct token_vocabulary* Lexic_Vocabulary_Make_File(char *file_name);
struct token_vocabulary* Lexic_Vocabulary_Make_Stream(char *stream);

/*Read File and Create a NULL-Terminated Token Stream*/
struct token_instance* Lexic_Token_Stream_File(char *file_name, struct token_vocabulary *vocab);
struct token_instance* Lexic_Token_Stream_String(char *stream, struct token_vocabulary *vocab);

/*Read File and Create a NULL-Terminated Name Stream (for interoperability)*/
char ** Lexic_Token_Name_Stream_File(char *file_name, struct token_vocabulary *vocab);
char ** Lexic_Token_Name_Stream_String(char *stream, struct token_vocabulary *vocab);

#endif
