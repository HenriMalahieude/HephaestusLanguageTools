#ifndef __HEPHAESTUS_REGEX__
#define __HEPHAESTUS_REGEX__
#include <stdbool.h>

bool RegexValidate(char *reg);

bool RegexMatch(char *reg, char *input);

#endif
