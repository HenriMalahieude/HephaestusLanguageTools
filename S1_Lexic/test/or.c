#include <stdio.h>
#include <string.h>

#include "../private/regex.h"

int regex_line_no = 0;
int regex_colu_no = 0;

int main(void) {

#ifdef VERBOSE
	printf("[!] Regex Direct Test\n");
#endif

	char match1[4] = "int";
	char match2[7] = "double";
	char match3[5] = "char";

	struct regex *direct1 = Regex_New_Direct(match1);
	struct regex *direct2 = Regex_New_Direct(match2);
	struct regex *direct3 = Regex_New_Direct(match3);

#ifdef VERBOSE
	printf("[.] Initialized Direct Regexes\n");
#endif

	struct regex *or1 = Regex_New_Or(direct1, direct2);
	struct regex *or2 = Regex_New_Or(or1, direct3);

#ifdef VERBOSE
	printf("[.] Initialized Or Regexes\n");
#endif

#ifdef VERBOSE
	printf("[?] Regex Or Test 1\n");
#endif

	char test[100]; test[99] = '\0';
	strcpy(test, "int");
	if (!or1->match_function(test, or1->attached_data)) {
		printf("[X] Regex Or Test 1 Fail w/ %s\n", test);
	}

#ifdef VERBOSE
	printf("[?] Regex Or Test 2\n");
#endif

	strcpy(test, "double");
	if (!or1->match_function(test, or1->attached_data)) {
		printf("[X] Regex Or Test 2 Fail w/ %s\n", test);
	}
	
#ifdef VERBOSE
	printf("[?] Regex Or Test 3\n");
#endif

	strcpy(test, "integer");
	if (or1->match_function(test, or1->attached_data)) {
		printf("[X] Regex Or Test 3 Fail w/ %s\n", test);
	}
	
#ifdef VERBOSE
	printf("[?] Regex Or Test 4\n");
#endif

	strcpy(test, "doubley");
	if (or1->match_function(test, or1->attached_data)) {
		printf("[X] Regex Or Test 4 Fail w/ %s\n", test);
	}

#ifdef VERBOSE
	printf("[?] Regex Or Test 5\n");
#endif

	strcpy(test, "int\0");
	if (!or2->match_function(test, or2->attached_data)) {
		printf("[X] Regex Or Test 5 Fail w/ %s\n", test);
	}

#ifdef VERBOSE
	printf("[?] Regex Or Test 6\n");
#endif

	strcpy(test, "double\0");
	if (!or2->match_function(test, or2->attached_data)) {
		printf("[X] Regex Or Test 6 Fail w/ %s\n", test);
	}

#ifdef VERBOSE
	printf("[?] Regex Or Test 7\n");
#endif

	strcpy(test, "char\0");
	if (!or2->match_function(test, or2->attached_data)) {
		printf("[X] Regex Or Test 7 Fail w/ %s\n", test);
	}

#ifdef VERBOSE
	printf("[?] Regex Or Test 8\n");
#endif

	strcpy(test, "characcter\0");
	if (or2->match_function(test, or2->attached_data)) {
		printf("[X] Regex Or Test 8 Fail w/ %s\n", test);
	}

#ifdef VERBOSE
	printf("[?] Regex Or Test 9\n");
#endif

	strcpy(test, "thingamajig\0");
	if (or2->match_function(test, or2->attached_data)) {
		printf("[X] Regex Or Test 9 Fail w/ %s\n", test);
	}

	#ifdef VERBOSE
	printf("[!] Regex Or Test Finished\n");
	#endif

	return 0;
}
