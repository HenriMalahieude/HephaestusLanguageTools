#include <stdio.h>
#include <string.h>
#include "../private/regex.h"

//#define VERBOSE
int regex_line_no = 0;
int regex_colu_no = 0;

int main(void) {
	#ifdef VERBOSE
	printf("[!] Regex Direct Test\n");
	#endif

	char match[4] = "int";
	struct regex *direct = Regex_New_Direct(match);

	#ifdef VERBOSE
	printf("[.] Initializing Direct Regex\n");
	#endif

	char test[100]; test[99] = '\0';
	strcpy(test, "i\0");

	#ifdef VERBOSE
	printf("[?] Regex Direct Test 1\n");
	#endif

	if (direct->match_function(test, direct->attached_data)) {
		printf("[X] Regex Direct Test 1 w/ %s\n", test);
	}

	#ifdef VERBOSE
	printf("[?] Regex Direct Test 2\n");
	#endif

	strcpy(test, "in\0");
	if (direct->match_function(test, direct->attached_data)) {
		printf("[X] Regex Direct Test 2 w/ %s\n", test);
	}

	#ifdef VERBOSE
	printf("[?] Regex Direct Test 3\n");
	#endif

	strcpy(test, "int\0");
	if (!direct->match_function(test, direct->attached_data)) {
		printf("[X] Regex Direct Test 3 w/ %s\n", test);
	}

	#ifdef VERBOSE
	printf("[?] Regex Direct Test 4\n");
	#endif

	strcpy(test, "double\0");
	if (direct->match_function(test, direct->attached_data)) {
		printf("[X] Regex Direct Test 4 w/ %s\n", test);
	}

	#ifdef VERBOSE
	printf("[?] Regex Direct Test 5\n");
	#endif

	strcpy(test, "dobby!!!");
	if (direct->match_function(test, direct->attached_data)) {
		printf("[X] Regex Direct Test 5 w/ non null termination\n");
	}

	#ifdef VERBOSE
	printf("[!] Regex Direct Test Finished\n");
	#endif
}
