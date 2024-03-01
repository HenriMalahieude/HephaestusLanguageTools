#ifndef __HEPHAESTUS_REGEX_MATCH__
#define __HEPHAESTUS_REGEX_MATCH__
#include <stdbool.h>

#define MATCH_FUNC(name) bool name(char *input, void *attached)

/*Match def "a" with input "a"*/
MATCH_FUNC(Regex_Match_Direct);

/*Match def "a|b" with either "a" or "b"*/
MATCH_FUNC(Regex_Match_Or);

/*Match [abc] with "a" or "b" or "c"*/
MATCH_FUNC(Regex_Match_Brackets);

/*Match "a-z" as a sequence from a to z, hardcoded for now*/
MATCH_FUNC(Regex_Match_Sequence);

/*Match "a?" with zero or one a, hardcoded for now*/
MATCH_FUNC(Regex_Match_Qualifier);

#endif
