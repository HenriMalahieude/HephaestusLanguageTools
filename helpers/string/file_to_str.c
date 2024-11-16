#include <stdio.h>
#include <stdlib.h>
#include "../log/warn.h"

//NOTE: Dynamically allocates a string. Expects it to be freed by caller
char* ftostr(char *file_name) {
	FILE *read_from = fopen(file_name, "r");
	if (read_from == NULL) HLT_AERR("Couldn't open file provided?");

	unsigned long long file_size = 0;

	int succ = fseek(read_from, 0, SEEK_END); //Move to eof
	if (succ != 0) {
		HLT_WRN("Couldn't seek file to the end?", HLT_MJRWRN);
		return NULL;
	}

	file_size = ftell(read_from); //Amount of characters
	rewind(read_from); //Back to top
	
	char *strm = (char*)calloc(sizeof(char), file_size); 
	if (strm == NULL) {
		fclose(read_from);
		HLT_AWRN("Failed to allocate memory for file?");
		return NULL;
	}

	succ = fread(strm, 1, sizeof(char)*file_size, read_from); //1 elm of size file
	if (succ <= 1) {
		fclose(read_from);
		free(strm);
		HLT_AWRN("Failed to read entire file into buffer?");
		return NULL;
	}
	
	fclose(read_from);
	return strm;
}
