#include <stdio.h>
#include <stdlib.h>

//NOTE: Dynamically allocates a string. Expects it to be freed by caller
char* ftostr(char *file_name) {
	FILE *read_from = fopen(file_name, "r");
	if (read_from == NULL) {
		printf("Could not read from file. fopen returned null, file doesn't exist?\n");
		exit(1);
	}

	unsigned long long file_size = 0;

	int succ = fseek(read_from, 0, SEEK_END); //Move to eof
	if (succ != 0) {
		printf("Could not seek file to end. (%d)\n", succ);
		exit(1);
	}

	file_size = ftell(read_from); //Amount of characters
	rewind(read_from); //Back to top
	
	char *strm = (char*)calloc(1, file_size); 
	if (strm == NULL) {
		fclose(read_from);
		printf("Failed to allocate memory (%lld bytes) for file, calloc returned null for strm!\n", file_size+1);
		exit(1);
	}

	succ = fread(strm, 1, file_size, read_from);
	if (succ <= 1) {
		fclose(read_from);
		free(strm);
		printf("Failed to read from supplied file (characters read <= 1): fread (%d), file_size (%lld)\n", succ, file_size);
		exit(1);
	}
	
	fclose(read_from);
	return strm;
}
