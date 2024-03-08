#ifndef __HEPHAESTUS_REGEX__
#define __HEPHAESTUS_REGEX__
#include <stdbool.h>

enum regex_type {
	RT_UNDEFINED,
	RT_DIRECT,
	RT_OR,
	RT_BRACKETS,
	RT_SEQUENCE,
	RT_QUALIFIER,
	RT_ESCAPED,
	RT_GROUP,
	RT_COUNT // amount of types
};

struct regex {
	enum regex_type type;
	void *attached_data;
	bool (*match_function)(char *input, void *attached);
};


//Feedback System
void Regex_Error(char *msg);
void Regex_Warning(char *msg);

//Create a Regex (creates a local copy of char* if handed a char*)
struct regex* Regex_New_Direct(char *match);
struct regex* Regex_New_Or(struct regex *left, struct regex *right);
struct regex* Regex_New_Brackets(char *internals); //[ ]
struct regex* Regex_New_Sequence(char *seq);
struct regex* Regex_New_Qualifier(struct regex *prev, char qualifier);
struct regex* Regex_New_Escaped(char *special);
struct regex* Regex_New_Group(char *internals); //( )

//Match a Regex
bool Regex_Match(struct regex *reg, char *input);

//Create a sequence of regexes
struct regex** Regex_New_Definition(char *definition, int *count);

//Free regex's
void Regex_Free(struct regex *reg);
#endif
