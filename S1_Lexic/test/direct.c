#define TEST_NAME "Direct"
#include "regex_helper.h"

void DirectTest() {
#ifdef VERBOSE
	printf("[!] Regex Direct Test Start\n");
#endif

	char match[4] = "int";
	struct regex direct = Regex_New_Direct(match);

#ifdef VERBOSE
	printf("[.] Initialized Direct Regex\n");
#endif


	TEST_REGEX(direct, "", false);
	
	TEST_REGEX(direct, "i", false);
	
	TEST_REGEX(direct, "in", false);

	TEST_REGEX(direct, "int", true);

	TEST_REGEX(direct, "double", false);

#ifdef VERBOSE
	printf("[!] Regex Direct Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	DirectTest();
	return 0;
}
#endif
