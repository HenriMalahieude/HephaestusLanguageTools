#ifndef __HEPHAESTUS_LEXICAL_ANALYZER__
#define __HEPHAESTUS_LEXICAL_ANALYZER__

typedef struct lexic_vocabulary LexicVocabulary;

typedef struct lexic_token {
	char *definition_name;
	char *matching_input;
	int line;
	int col;
} LexicToken;

/*                       Vocabulary Section                       */

//Allocate and Populate from a File
LexicVocabulary * Lexic_Vocabulary_Make_File(char *file_name);

//Allocate and Populate from a Character Stream
LexicVocabulary * Lexic_Vocabulary_Make_Stream(char *stream);

//Allocate an empty Vocabulary Set
LexicVocabulary * Lexic_Vocabulary_Allocate();

//Add a new definition to a Vocabulary Set (makes a local dynamic copy of name and regex params)
void Lexic_Vocabulary_Add_Definition(LexicVocabulary *vocab, char *name, char *regex);

//Free a Vocabulary
void Lexic_Vocabulary_Free(Lexic_Vocabulary *);


/*                       Token Stream Section                       */

//Allocate and Populate a null-terminated Token Stream from a File
LexicToken * Lexic_Token_Stream_File(char *file_name, LexicVocabulary *vocab);

//Allocate and Populate a null-terminated Token Stream from a Character Stream
LexicToken * Lexic_Token_Stream_String(char *stream, LexicVocabulary *vocab);

//Allocate and Populate a null-terminated Token Name Stream from a File
char ** Lexic_Token_Name_Stream_File(char *file_name, LexicVocabulary *vocab);

//Allocate  and Populate a null-terminated Token Name Stream from a Character Stream
char ** Lexic_Token_Name_Stream_String(char *stream, LexicVocabulary *vocab);

#endif
