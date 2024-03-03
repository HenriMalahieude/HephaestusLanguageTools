#include "regex_helper.h"

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

	char test[100]; test[99] = '\0';

	//Double Or
	TEST_REGEX_TRUE("Or", 1, test, "int", or1);

	TEST_REGEX_TRUE("Or", 2, test, "double", or1);
	
	TEST_REGEX_FALSE("Or", 3, test, "integer", or1);
	
	TEST_REGEX_FALSE("Or", 4, test, "doubley", or1);

	TEST_REGEX_FALSE("Or", 5, test, "not", or1);

	//Triple Or
	TEST_REGEX_TRUE("Or", 6, test, "int", or2);

	TEST_REGEX_TRUE("Or", 7, test, "double", or2);

	TEST_REGEX_TRUE("Or", 8, test, "char", or2);

	TEST_REGEX_FALSE("Or", 9, test, "in", or2);

	TEST_REGEX_FALSE("Or", 10, test, "oubl", or2);

	TEST_REGEX_FALSE("Or", 11, test, "ha", or2);

	TEST_REGEX_FALSE("Or", 12, test, "bruh", or2);

#ifdef VERBOSE
	printf("[!] Regex Or Test Finished\n");
#endif

	return 0;
}
