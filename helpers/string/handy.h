#ifndef __HEPH_HANDY_STRING_FUNCS__
#define __HEPH_HANDY_STRING_FUNCS__
#include <stdbool.h>

//Caller expected to free the allocation
char *ftostr(char *file_name);

//Caller expected to free the allocation
//returns if wasn't able to create a valid string (even just '\0' is considered valid)
bool trim(char *input, char **nstr);

#endif
