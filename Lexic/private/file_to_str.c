#include "warn.h"

//NOTE: Dynamically allocates a string. Expects it to be freed by caller
char* ftostr(char *file_name) {
	FILE *read_from = fopen(file_name, "r");
	if (read_from == NULL) Lexic_Error("Could not read from file. Does it exist?");

	long file_size = 0;
	fseek(read_from, 0, SEEK_END); //Move to eof
	file_size = ftell(read_from); //Amount of characters
	rewind(read_from); //Back to top
	
	char *strm = (char*)calloc(1, file_size+1); 
	if (strm == NULL) {
		fclose(read_from);
		Lexic_Error("Failed to allocate memory for file");
	}
	
	if (1 != fread(strm, file_size, 1, read_from)) {
		fclose(read_from);
		free(strm);
		Lexic_Error("Failed to read from supplied file");
	}

	fclose(read_from);
	return strm;
}
