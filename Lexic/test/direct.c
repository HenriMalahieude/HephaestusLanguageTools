#define TEST_NAME "Direct"
#include "regex_helper.h"

void DirectTest() {
#ifdef VERBOSE
	printf("[!] Regex Direct Test Start\n");
#endif

	char match[4] = "int";
#ifdef VERBOSE
	printf("[.] Initialized Direct Regex\n");
#endif


	TEST_REGEX(match, "", false);
	
	TEST_REGEX(match, "i", false);
	
	TEST_REGEX(match, "in", false);

	TEST_REGEX(match, "int", true);
	
	TEST_REGEX(match, "integer", false);

	TEST_REGEX(match, "double", false);

#ifdef VERBOSE
	printf("[!] Regex Direct Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	//warn_level = LWT_DEBUG;
	DirectTest();
	return 0;
}
#endif
