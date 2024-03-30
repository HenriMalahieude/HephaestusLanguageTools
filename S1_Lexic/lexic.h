#ifndef __HEPHAESTUS_LEXICAL_ANALYZER__
#define __HEPHAESTUS_LEXICAL_ANALYZER__

extern int regex_line_no;
extern int regex_colu_no;

enum lex_warn_type {
	LWT_SILENT = 0,
	LWT_MJRWRN,
	LWT_STDWRN,
	LWT_VERBSE,
	LWT_DEBUG
};

extern enum lex_warn_type warn_level; //only warn about this and more important
void Lexic_Error(char *msg);
void Lexic_Warn(char *msg, enum lex_warn_type type);

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

/*An Instance of a Token*/
struct token_instance {
	char *definition_name;
	char *matching_input;
	int line;
	int col;
};

/*Make Lexical Vocabulary*/
struct token_vocabulary Lexic_Vocabulary_Make_File(char *file_name);
struct token_vocabulary Lexic_Vocabulary_Make_Stream(char *stream);

/*Read File and Create a NULL-Terminated Token Stream*/
struct token_instance* Lexic_Token_Stream_File(char *file_name, struct token_vocabulary *vocab);
struct token_instance* Lexic_Token_Stream_String(char *stream, struct token_vocabulary *vocab);

/*Read File and Create a NULL-Terminated Name Stream (for interoperability)*/
char ** Lexic_Token_Name_Stream_File(char *file_name, struct token_vocabulary *vocab);
char ** Lexic_Token_Name_Stream_String(char *stream, struct token_vocabulary *vocab);

#endif
