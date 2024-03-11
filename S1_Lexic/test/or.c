#define TEST_NAME "Or"
#include "regex_helper.h"

void OrTest() {

#ifdef VERBOSE
	printf("[!] Regex Or Test Start\n");
#endif

	char match1[4] = "int";
	char match2[7] = "double";
	char match3[5] = "char";

	struct regex direct1 = Regex_New_Direct(match1);
	struct regex direct2 = Regex_New_Direct(match2);
	struct regex direct3 = Regex_New_Direct(match3);

#ifdef VERBOSE
	printf("[.] Initialized Direct Regexes\n");
#endif

	struct regex or1 = Regex_New_Or(direct1, direct2);
	struct regex or2 = Regex_New_Or(or1, direct3);

#ifdef VERBOSE
	printf("[.] Initialized Or Regexes\n");
#endif


	//Double Or
	TEST_REGEX(or1, "int", true);

	TEST_REGEX(or1, "double", true);
	
	TEST_REGEX(or1, "integer", false);
	
	TEST_REGEX(or1, "doubley", false);

	TEST_REGEX(or1, "not", false);

	//Triple Or
	TEST_REGEX(or2, "int", true);

	TEST_REGEX(or2, "double", true);

	TEST_REGEX(or2, "char", true);

	TEST_REGEX(or2, "in", false);

	TEST_REGEX(or2, "oubl", false);

	TEST_REGEX(or2, "har", false);

	TEST_REGEX(or2, "bruh", false);

#ifdef VERBOSE
	printf("[!] Regex Or Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	OrTest();
	return 0;
}
#endif
