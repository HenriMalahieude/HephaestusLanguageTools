#ifndef __HEPHAESTUS_REGEX__
#define __HEPHAESTUS_REGEX__
#include <stdbool.h>

bool Regex_Validate(char *reg);

bool Regex_Match(char *reg, char *input);
#endif
