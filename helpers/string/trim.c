#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//probably stolen from stack overflow, can't remember
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

	*nstr = calloc(len+1, sizeof(char));
	strncpy(*nstr, input+front, len);
	return true;
}
