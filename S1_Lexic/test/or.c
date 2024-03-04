#include "regex_helper.h"

#undef TEST_NAME
#define TEST_NAME "Or"

void OrTest() {

#ifdef VERBOSE
	printf("[!] Regex Or Test Start\n");
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


	//Double Or
	TEST_REGEX_TRUE("int", or1);

	TEST_REGEX_TRUE("double", or1);
	
	TEST_REGEX_FALSE("integer", or1);
	
	TEST_REGEX_FALSE("doubley", or1);

	TEST_REGEX_FALSE("not", or1);

	//Triple Or
	TEST_REGEX_TRUE("int", or2);

	TEST_REGEX_TRUE("double", or2);

	TEST_REGEX_TRUE("char", or2);

	TEST_REGEX_FALSE("in", or2);

	TEST_REGEX_FALSE("oubl", or2);

	TEST_REGEX_FALSE("ha", or2);

	TEST_REGEX_FALSE("bruh", or2);

#ifdef VERBOSE
	printf("[!] Regex Or Test Finished\n");
#endif
}

#ifndef ALL_TESTS
int main(void) {
	OrTest();
	return 0;
}
#endif
