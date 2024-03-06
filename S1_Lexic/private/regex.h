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


//Create a Regex

struct regex* Regex_New_Direct(char *match);

struct regex* Regex_New_Or(struct regex *left, struct regex *right);

struct regex* Regex_New_Brackets(char *internals);

struct regex* Regex_New_Sequence(char *seq);

struct regex* Regex_New_Qualifier(struct regex *prev, char qualifier);

struct regex* Regex_New_Escaped(char *special);


//Create a sequence of regex's
struct regex** Regex_New_Definition(char *definition, int *count);

#endif
