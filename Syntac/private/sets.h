#include <stdbool.h>
#include <stdarg.h>

#ifndef __HEPH_SETS__
#define __HEPH_SETS__

// Sets of strings (unique, case-sensitive)
// Sets must be NULL-Terminated

// Creates a new set of strings, 
// where count is the total elements within set (NULL termination not include)
char ** SetCreate(int count, ...);

// Set contains string, yes or no
bool SetContains(char **set, char *item);

// Equivalency check (must be exact same)
bool SetEquality(char **set1, char **set2);

// Outputs succesful (and unique) additions to the set
// If it's in the set before, it returns false
// Makes a (malloc) copy of the item
bool SetAdd(char ***set, char *item);

// Returns a dynamically allocated set union between set1 & set2
char ** SetUnion(char **set1, char **set2);

// Amount of elements in set
int SetCount(char **set);

// Output the elements in the set
void SetPrint(char **set);

// Delete and reset set (free the memory too)
void SetFree(char **set);

#endif
