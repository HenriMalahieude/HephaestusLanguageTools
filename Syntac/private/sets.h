#include <stdbool.h>

#ifndef __HEPH_SETS__
#define __HEPH_SETS__

//Sets of strings (unique, case-sensitive)
//Sets must be NULL-Terminated

//Set contains string, yes or no
bool SetContains(char **set, char *item);

//Outputs succesful (and unique) additions to the set
//If it's in the set before, it returns false
//Makes a (malloc) copy of the item
bool SetAdd(char ***set, char *item);

//Amount of elements in set
int SetCount(char **set);

//Delete and reset set (free the memory too)
void SetFree(char **set);

#endif
