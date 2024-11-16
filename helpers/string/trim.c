#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//probably stolen from stack overflow, can't remember

//trims whitespace from front and back of 'input', places result in nstr
//returns whether input was not all whitespace
bool trim(char *input, char **nstr) {
	int ilen = strlen(input);
	int front = 0;
	
	for (front = 0; front < ilen; front++) {
		if (!isspace(input[front])) break;
	}

	if (front >= ilen) return false;

	int back = front;
	for (back = front; back < ilen; back++) {
		if (isspace(input[back+1])) break;
	}
	
	int len = (back - front) + 1;
	if (len <= 0) return false;

	*nstr = (char*)calloc(len+1, sizeof(char));
	if (*nstr == NULL) return false;

	strncpy(*nstr, input+front, len);
	return true;
}
