#ifndef __HEPHAESTUS_REGEX__
#define __HEPHAESTUS_REGEX__
#include <stdbool.h>

struct regex {
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

struct regex* Regex_New_Qualifier(struct regex *prev);

struct regex* Regex_New_Escaped(char *special);

#endif
