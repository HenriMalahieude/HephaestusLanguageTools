#ifndef __HEPHAESTUS_LEXICAL_ANALYZER__
#define __HEPHAESTUS_LEXICAL_ANALYZER__
#include <stdbool.h>

//Container of definition regex's
//internal access restricted to "private/lexic_internal.h"
typedef struct lxc_vocabulary LexicVocabulary;

typedef struct lxc_token {
	char *definition_name;
	char *matching_input;
	int line;
	int col;
} LexicToken;

/*----------------------------Vocabulary----------------------------*/

//Allocate and Populate a vocabulary from a File
//Note that this function will load the entire file into a char*
//and then calls FromString on the loaded string before freeing it.
LexicVocabulary * LexicVocabularyFromFile(char *file_name);

//Allocate and Populate a vocabulary from a String/Stream
LexicVocabulary * LexicVocabularyFromString(char *stream);

//Allocate an empty Vocabulary
LexicVocabulary * LexicVocabularyAllocate();

//Add a new definition to a Vocabulary Set (makes a local dynamic copy of name and regex params)
//Returns whether the regex was valid and added to vocab, does not check for duplicates
bool LexicVocabularyDefinitionAdd(LexicVocabulary *vocab, char *name, char *regex);

//Free a Vocabulary
void LexicVocabularyFree(LexicVocabulary *);

/*----------------------------Token Stream----------------------------*/

//Allocate and Populate a null-terminated (by definition_name field) Token Stream from a File
LexicToken * LexicTokensFromFile(char *file_name, LexicVocabulary *vocab);

//Allocate and Populate a null-terminated (by definition_name field) Token Stream from a Character Stream
LexicToken * LexicTokensFromString(char *stream, LexicVocabulary *vocab);

//Free a null-terminated Token Stream
void LexicTokensFree(LexicToken *);

/*----------------------------Token Name Stream----------------------------*/

//Allocate and Populate a null-terminated Token Name Stream from a File
char ** LexicTokenNamesFromFile(char *file_name, LexicVocabulary *vocab);

//Allocate and Populate a null-terminated Token Name Stream from a Character Stream
char ** LexicTokenNamesFromString(char *stream, LexicVocabulary *vocab);

//Free a null-terminated Token Name Stream
void LexicTokenNamesFree(char **);

#endif
