#ifndef __HEPHAESTUS_LEXICAL_ANALYZER__
#define __HEPHAESTUS_LEXICAL_ANALYZER__

typedef struct token_vocabulary LexicVocabulary;

typedef struct token_instance {
	char *definition_name;
	char *matching_input;
	int line;
	int col;
} LexicToken;

/*Make Lexical Vocabulary*/
LexicVocabulary * Lexic_Vocabulary_Make_File(char *file_name);
LexicVocabulary * Lexic_Vocabulary_Make_Stream(char *stream);

/*Read File and Create a NULL-Terminated Token Stream*/
LexicToken * Lexic_Token_Stream_File(char *file_name, LexicVocabulary *vocab);
LexicToken * Lexic_Token_Stream_String(char *stream, LexicVocabulary *vocab);

/*Read File and Create a NULL-Terminated Name Stream (for interoperability)*/
char ** Lexic_Token_Name_Stream_File(char *file_name, LexicVocabulary *vocab);
char ** Lexic_Token_Name_Stream_String(char *stream, LexicVocabulary *vocab);

#endif
