#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sets.h"

bool SetContains(char **set, char *item) {
	if (set == NULL || item == NULL) return false;

	int count = SetCount(set);
	for (int i = 0; i < count; i++) {
		char *elm = set[i];
		if (strcmp(item, elm) == 0) return true;
	}

	return false;
}

//One by one add, may not be totally efficient
//...
//But honestly who cares, I got better things to do
//NOTE: Could track capacity with special 0x1, 0x2, 0x0 "pointer" values
//		But that's a bit too overkill for this lmao
bool SetAdd(char ***set, char *item) {
	if (item == NULL || set == NULL) return false;
	if (SetContains(*set, item)) return false;

	int count = SetCount(*set);
	char **tmp = malloc(sizeof(char **) * (count+2));
	if (tmp == NULL) return false; //malloc failed?
	
	for (int i = 0; i < count; i++) {
		tmp[i] = (*set)[i]; //copy over the pointers to the strings
	}
	
	tmp[count] = calloc(strlen(item)+1, sizeof(char));
	strcpy(tmp[count], item);
	tmp[count+1] = NULL;

	if (*set != NULL) free(*set); //only free what has been jailed
	*set = tmp;

	return true;
}

int SetCount(char **set) {
	if (set == NULL) return 0;

	int cnt;
	for (cnt = 0; set[cnt] != NULL; cnt++);

	return cnt;
}

void SetFree(char **set) {
	if (set == NULL) return;

	int count = SetCount(set);
	for (int i = 0; i < count; i++) {
		free(set[i]);
		set[i] = NULL;
	}

	free(set);
}
